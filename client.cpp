/*
** client.cpp -- a stream socket client demo
*/

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::to_string;
using std::cin;

const int MAXDATASIZE{1000}; // max number of bytes we can get at once

void *get_in_addr(struct sockaddr *sa){ // ottiene sockaddr, IPv4 o IPv6:
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}
	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{

	if (argc != 3) {	//controlla di avere tutti i parametri altrimenti da errore
	    cerr << "usage: ./client hostname port\n";
	    exit(EXIT_FAILURE);
	}

	int sockfd, numbytes;
	char buf[MAXDATASIZE];	//per memorizzare i dati ricevuti dal server
	struct addrinfo hints, *servinfo, *p;	//strutture per la connessione
	int rv;
	char s[INET6_ADDRSTRLEN];


	memset(&hints, 0, sizeof(hints));	//riempie la struttura con le informazioni
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
		cerr <<  "getaddrinfo: " << gai_strerror(rv) << endl;
		exit(EXIT_FAILURE);
	}

	for(p = servinfo; p != NULL; p = p->ai_next) {	//cicla su tutti i risultati e tiene il primo con cui si può fari il bind

		if ((sockfd = socket(p->ai_family, p->ai_socktype,p->ai_protocol)) == -1) {	// crea la socket -> socket(int domain, int type, int protocol)
			cerr << "Failed to create socket:\n";	//da errore se fallisce
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {	//si connette alla porta del server
			close(sockfd);	//chiude la socket se fallisce
			cerr << "Failed to connect to socket..."; //da errore se fallisce
			continue;
		}

		break;
	}

	if (p == NULL) {	//se alla fine del ciclo non è riuscito a fare la socket da errore
		cerr <<  "client: failed to connect\n";
		exit(EXIT_FAILURE);
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),s, sizeof(s));
	cout << "client: connecting to " << s << endl;

	freeaddrinfo(servinfo); //libera la struttura

	if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {	//riceve la domanda della scatola da parte del server
	    cerr << "Client failed to receive data\n";	//se fallisce da errore
	    exit(EXIT_FAILURE);
	}

	buf[numbytes] = '\0';

	cout << buf <<  endl;	//stampa la domanda ricevuta dal server

	int richiesta = 0;
	cout << "Inserire un numero: " << endl;	//input per la scatola di cui si vogliono le informazioni
	cin >> richiesta;
	string str = to_string(richiesta);	//trasforma int in stringa

	if (send(sockfd, str.c_str(), 5, 0) == -1){	//manda il numero della scatola al server
    cerr << "send  \n";	//se fallisce da errore
  }

	if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {	//riceve la stringa con le informazioni sulla scatola da parte del server
	    cerr << "Client failed to receive data\n";	//se fallisce da errore
	    exit(EXIT_FAILURE);
	}

	buf[numbytes] = '\0';

	cout << buf << endl;	//stampa la stringa con le informazioni

	close(sockfd);	//chiude la socket

	return 0;
}
