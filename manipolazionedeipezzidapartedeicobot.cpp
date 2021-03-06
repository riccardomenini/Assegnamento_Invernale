#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <mutex>
#include <thread>
#include <chrono>
#include "globali.h"
using std::string;
using std::vector;
using std::ifstream;
using std::cout;
using std::mutex;
using std::condition_variable;
using std::endl;
using std::unique_lock;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;

void leggifile(string nome, vector<string> & lines){	//funzione di lettura file. Il vettore viene passato per riferimento in quanto √® molto
																											//grande e la copia sarebbe dispendiosa
	string a_line;
	ifstream myfile("arrivi_linea_"+ nome +".txt");	//apertura file
	if (!myfile.is_open()) { //errore di apertura
		unique_lock<mutex> mlockcout(mutexcout_);	//il cout non √® thread safe e quindi viene protetto
		cout << "unable to open file: " << "arrivi_linea_"+ nome +".txt" << "\n";
		mlockcout.unlock();
		exit(EXIT_FAILURE);
	}

	lines = {};

	while (getline (myfile, a_line)){
		lines.push_back(a_line);	//lettura riga per riga
	}

	myfile.close();	//chiusura file

}

void manipolazionedeipezzidapartedeicobot(const vector<int> & veldis, string nome){
	int lineelette = 0;
	vector<string> lines; //vettore che contiene le linee lette dal file
	int s = 0;

	if (nome == "a"){
		s = (veldis[0]/veldis[1]);
	}else{
		s = (veldis[2]/veldis[3]);
	}
	sleep_for(milliseconds{s*10}); //s*10 tempo per arrivare al cobot del primo pezzo

	while(lineelette < MAX && stato){	//controllo di non aver letto MAX linee (quindi quelle massime che possono essere scritte) e che il sitema
																		//non sia spento

		unique_lock<mutex> mlockfout(mutexfout_);	//√® il semaforo che serve per il wait sotto inoltre insieme al semaforo presente in arrivo dei pezzi
																							//permette solo la scrittura o la lettura del file e mai la contemporaneit√† delle due
		leggifile(nome,lines);	//lettura

  	while (lines.size() == lineelette){	//mi fermo se ho letto tutte le linee che sono scritte nel file. Uso while per risvegli spuri
			not_empty_.wait(mlockfout);	//se ho gi√† letto tutte le linee mi fermo
			leggifile(nome,lines);	//appena vengo risvegliato rileggo tutto
		}
		mlockfout.unlock();

    for (int i = lineelette; i < lines.size(); i++){	//parto dall'ultima linea letta

      size_t found1 = lines[i].find_first_of("ABC");	//parametri cobot
      size_t found2 = lines[i].find_first_of("0123456789",found1);
      size_t found3 = lines[i].find_first_of("0123456789");
      size_t found4 = lines[i].find_first_not_of("0123456789");
      size_t found5 = lines[i].find_first_of("0123456789",found4);
      size_t found6 = lines[i].find_first_not_of("0123456789",found5) - 2;
      int mm = stoi(lines[i].substr(found3, found4));
      int ss = stoi(lines[i].substr(found5, found6));
      string pos = lines[i].substr(found2, string::npos);

      if (nome == "a"){	//calcolo tempo tra sensore e cobot
        ss += (veldis[0]/veldis[1]) + mm * 60;
				mm = floor(ss/60.);
				ss = ss%60;
      }else{
				ss += (veldis[2]/veldis[3]) + mm * 60;
				mm = floor(ss/60.);
				ss = ss%60;
      }

			sleep_for(milliseconds{20});	//azione cobot 10ms + i pezzi sono distanti 10ms in quanto venivano creati dalla thread arrivo dei pezzi ogni 10ms

			unique_lock<mutex> mlockinserimentopezzi(mutexinserimentopezzi_); //modifico delle variabili globali quindi proteggo con un semaforo
			pezziinnum ++;	//il pezzo viene inserito nella scatola

			unique_lock<mutex> mlockcout(mutexcout_); //il cout non √® thread safe e quindi viene protetto
			cout << "Cobot linea di trasporto " << nome << ": recuperato componente di tipo " << lines[i][found1] << " al tempo " << mm << " " << ss << " in posizione " << pos << " e inserito nella scatola " << num << '\n';
			mlockcout.unlock();

			lineelette = lines.size();	//il valore di linee lette viene incrementato quando il cobot agisce su quei pezzi

			tipologie[num][pezziinnum - 1] = lines[i][found1];	//aggiorno il magazzino con il contenuto della scatola. Viene fatto qui e non dopo il
			orario[num][pezziinnum - 1] = mm * 60 + ss;					//trasporto in magzzino da parte dei cobot perch√® solo qui ho le informazioni su cosa contiene

			if (pezziinnum > 9){	//se √® maggiore di 9 cambio scatola

				unique_lock<mutex> mlockpostazioneoccupata(mutexpostazioneoccupata_);	//semaforo per wait sotto. Inoltre protegge la modifica di
																																							//postazioneoccupata rispetto al robot mentre mlockinserimentopezzi
																																							//rispetto all'altro cobot

				postazioneoccupata = true;	//la postazione √® occupata da una scatola piena
				postazioneoccupata_.notify_one();	//segnalo ai cobot che la postazione √® occupata da una scatola piena

				while (postazioneoccupata){ //Uso while per risvegli spuri. Rimango bloccato fino a quando i robot trasportatori non liberano la postazione
					postazioneoccupata_.wait(mlockpostazioneoccupata);	//se la postazione √® occupata da una scatola piena mi blocco
																															//non si dovrebbe fare la wait in sezione critica (data da inserimentopezzi)
																															//ma in questo caso ha senso perch√® se una linea non pu√≤ proseguire, neanche l'altra
																															//linea pu√≤ farlo in quanto la postazione √® piena e quindi entrambe devono aspettare.
																															//Viene usato un mutex diverso da mutexinserimentopezzi_ in quanto se fosse stato
																															//utilizzato quello quando la wait si attivava sbloccava tale mutex e l'altro cobot che
																															//era fermo prima dell'istruzione "pezziinnum ++;" proseguiva immettendo oggetti nelle
																															//scatole gi√† piene.
				}
				mlockpostazioneoccupata.unlock();

				pezziinnum = pezziinnum % 10; //nella scatola nuova ci sono 0 pezzi
				num ++;	//La postazione √® libera quindi posso mettere una scatola nuova

				mlockinserimentopezzi.unlock();
			}
		}

  }

	unique_lock<mutex> mlockfineproduzione(mutexfineproduzione_);	//modifico una variabile globale quindi proteggo con un semaforo
	fineproduzione ++;	//Questa linea ha finito la produzione

	if (fineproduzione == 2){//Se entrambe le linee hanno finito la produzione posso sbloccare i robot e tirare via la scatola anche se non √® piena

		postazioneoccupata = true;	//tiro via la scatola anche se non √® piena
		postazioneoccupata_.notify_all(); //la condizione postazione occupata √® variata

	}

	mlockfineproduzione.unlock();
}
