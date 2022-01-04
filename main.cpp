#include <thread>
#include <math.h>
#include <iostream>
#include <vector>
#include "arrivodeipezzi.h"
#include "manipolazionedeipezzidapartedeicobot.h"
#include "riempimentodellescatole.h"
#include "interrogazionedaremoto.h"
#include "sistemadigracefuldegradation.h"
#include "globali.h"
using std::vector;
using std::thread;
using std::cout;
using std::endl;

mutex mutexfout_;
mutex mutexcout_;
mutex mutexpostazioneoccupata_;
mutex mutexfineproduzione_;
mutex mutexinserimentopezzi_;
condition_variable not_empty_;
condition_variable postazioneoccupata_;
int num = 0;
int pezziinnum = 0;
bool postazioneoccupata = false;
int fineproduzione = 0;
vector<vector<string>> tipologie ((ceil(2*MAX/10.) + 1) ,vector<string>(10));
vector<vector<int>> orario ((ceil(2*MAX/10.) + 1) ,vector<int>(10));
bool stato = true;
int numerosocket = 0;
bool socketaperta = false;


int main(int argc, char* argv[]){

  if (argc!= 6) { //controlla che ci siano tutti i parametri
		cout << "missing command line arguments:\n";
		cout << "to run ./main distanza_a velocità_a distanza_b velocità_b port\n";
		exit(EXIT_FAILURE);
	}

  vector<int> veldis; //trasformo argv in un vettore di interi
  for (int i = 1; i < 5; i++){
    veldis.push_back(atoi(argv[i]));
  }

  thread sistemadigracefuldegradation1{sistemadigracefuldegradation}; //thread per il controllo di ctrl c

  thread accept_loop1{accept_loop,argv[5]}; //thread per la creazione del server per la socket stream

  thread arrivodeipezzi1{arrivodeipezzi, "a"};  //thread per la generazione dei pezzi
	thread arrivodeipezzi2{arrivodeipezzi, "b"};

  thread manipolazionedeipezzidapartedeicobot1{manipolazionedeipezzidapartedeicobot, veldis, "a"};  //thread per il controllo del cobot
  thread manipolazionedeipezzidapartedeicobot2{manipolazionedeipezzidapartedeicobot, veldis, "b"};

  thread riempimentodellescatole1{riempimentodellescatole, 0};  //thread per il trasporto delle scatole a magazzino
  thread riempimentodellescatole2{riempimentodellescatole, 1};

	arrivodeipezzi1.join();                          //chiusura thread
	arrivodeipezzi2.join();

  manipolazionedeipezzidapartedeicobot1.join();
  manipolazionedeipezzidapartedeicobot2.join();

  riempimentodellescatole1.join();
  riempimentodellescatole2.join();

  accept_loop1.join();

  if (!stato){ //controlla che lo il sitema non sia stato spento

    cout << "\nMAGAZZINO" << endl;
    for (int i = 0; i < tipologie.size(); i++){ //stampa l'intero contenuto del magazzino
      cout << "Scatola " << i << endl;
      cout << "Tipologia pezzo \t";
      for (int j = 0; j < 10; j++){
        cout << tipologie[i][j] << "\t";
      }
      cout << endl;
      cout << "Orario \t\t\t";
      for (int j = 0; j < 10; j++){
        cout << orario[i][j] << "\t";
      }
      cout << endl;
    }

  }

  sistemadigracefuldegradation1.join();

  return 0;
}
