#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <iostream>
#include <vector>
#include <thread>
#include "globali.h"
using std::thread;
using std::cerr;
using std::cout;
using std::endl;
using std::vector;
using std::to_string;
using std::unique_lock;

const int MAXDATASIZE{100}; //numero massimo di byte che possono essere scambiati

void new_connection (int sock){

  if (send(sock, "Di quale scatola si vogliono sapere le informazioni?", 52, 0) == -1){ //il server chiede al client il numero della scatola
    cerr << "send  \n"; //se la send fallisce da errore
  }

  char buf[MAXDATASIZE];  //conterrà il messaggio ricevuto
  int numbytes; //dimensione messaggio ricevuto

  if ((numbytes = recv(sock, buf, MAXDATASIZE-1, 0)) == -1) { //riceve il messaggio contenente il numero della scatola dal client
      cerr << "Server failed to receive data\n";  //se fallisce la ricezione da errore
      exit(EXIT_FAILURE); //esce dal programma
  }

  int n = atoi(buf);  //trasformo il messaggio in un intero

  string dati = "";

  if (n > num) { //controllo che la scatola sia già in magazzino

      cerr << "Questa scatola non esiste in magazzino\n";
      dati = "Questa scatola non esiste in magazzino\n";

  }else{

    unique_lock<mutex> mlockcout(mutexcout_); //il cout non è thread safe e quindi viene protetto
    cout << "server: ricevuta richiesta per scatola "<< atoi(buf) << endl; //stampo dal server la richiesta ricevuta
    mlockcout.unlock();

    dati = "";

    dati += "Scatola " + to_string(n) + "\n"; //ricerco in magazzino le informazioni sulla richiesta ricevuta e constuisco la stringa per il client
    dati += "Tipologia pezzo \t";
    for (int j = 0; j < 10; j++){
      dati += tipologie[n][j] + "\t";
    }
    dati += "\n" ;
    dati += "Orario \t\t\t";
    for (int j = 0; j < 10; j++){
      dati += to_string(orario[n][j]/60) + " " + to_string(orario[n][j]%60) + "\t";
    }
    dati += "\n";

  }

  if (send(sock, dati.c_str(), 250, 0) == -1){  //mando la stringa creata al client
    cerr << "send  \n"; //se fallisce l'invio da errore
  }

  close(sock);  //Chiude la socket
}



void *get_in_addr(struct sockaddr *sa)  // ottiene sockaddr, IPv4 o IPv6:
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int make_accept_sock (const char* port) {

    struct addrinfo hints;  //struttura con le informazioni
    memset(&hints, 0, sizeof(hints) );  //riempie la struttura con le informazioni
    hints.ai_family   = AF_UNSPEC; // PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE; // use my IP

    struct addrinfo* servinfo;
    int rv = getaddrinfo(NULL, port, &hints, &servinfo);
    if (rv != 0) {
        cerr << "getaddrinfo:" << gai_strerror(rv) << endl;
        exit(EXIT_FAILURE);
    }

    struct addrinfo  *p;
    int sockfd; int yes{1};

		for(p = servinfo; p != NULL; p = p->ai_next) {//cicla su tutti i risultati e tiene il primo con cui si può fari il bind


			if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {// crea la socket -> socket(int domain, int type, int protocol)
				cerr << "server: failed to create socket\n";
				continue;
			}

			if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,sizeof(int)) == -1) {//
				cerr << "server: failed setsockopt\n";
				exit(EXIT_FAILURE);
			}


			if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {// bind(int fd, struct sockaddr *local_addr, socklen_t addr_length)
                                                          // passa il file descriptor, la struttura degli indirizzi creata e la sua linghezza, lega
                                                          // la socket all'indirizzo IP della porta
        close(sockfd);//se fallisce chiude la socket
				cerr << "server: failed bind\n";
				continue;
			}

			break;
		}

		if (p == NULL)  { //se alla fine del ciclo non è riuscito a fare la socket da errore
			cerr << "server: failed to bind\n";
			exit(EXIT_FAILURE);
		}

		freeaddrinfo(servinfo); //libera la struttura

  	return sockfd; //ritorna "l'id" della socket

}


void accept_loop (const char* port) {

    int sock = make_accept_sock(port);  //salva l'indirizzo della socket creata
    numerosocket = sock;  //salvo l'id per chiuderla con il ctrl c
    socketaperta = true;  //la socket è aperta

    while(true) {

    	const int BACKLOG{10};	 // quante connessioni posso avere in coda
			if (listen(sock, BACKLOG) == -1) {  //listen() dice alla socket di "ascoltare" i tentativi di connessione che arrivano e le mette in una
                                          //backlog queue fino a che accept() accetta la connessione
				cerr << "server: failed to listen\n"; //se fallisce da erroe
	    	exit(EXIT_FAILURE);
			}

      int new_sock = accept(sock, 0, 0);  //accetta una connessione arrivata e scrive le informazioni del cliente che si sta connettendo nella
                                          //struttura e aggiorna la dimensione della struttura. Questa funzione ritorna un file descriptor per
                                          //la connessione che è stata accettata in modo che il file descriptor originale possa essere ancora usato
                                          //per accettare nuove connessioni mentre quello nuovo possa essere usato per comunicare con il client

      if (new_sock < 0) { //se la socket non viene creata da errore
          cerr << "server: failed on accept\n";
          exit(EXIT_FAILURE);
      }
      std::thread t(new_connection, new_sock); // fa partire una nuova thread per la nuova connessione con il cliente mentre mantiene quella vecchia
                                               // per le nuove connessioni. Con un solo client non serve ma qui si suppone di avere più client
      t.detach(); // scollega le due thread
    }
}
