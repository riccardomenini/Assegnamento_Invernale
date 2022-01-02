#include <thread>
#include <math.h>
#include <iostream>
#include <vector>
#include "arrivodeipezzi.h"
#include "manipolazionedeipezzidapartedeicobot.h"
#include "riempimentodellescatole.h"
#include "interrogazionedaremoto.h"
#include "globali.h"
using std::vector;
using std::thread;
using std::cout;
using std::endl;

mutex mutex_;
mutex mutexcout_;
mutex mutexletturafile_;
mutex mutexnum_;
mutex mutexrobot_;
mutex mutexpostazioneoccupata_;
mutex mutexfineproduzione_;
mutex mutexmagazzino_;
condition_variable not_empty_;
vector<condition_variable> robot_(2);
condition_variable postazioneoccupata_;
int num = 0;
int pezziinnum = 0;
bool postazioneoccupata = false;
vector<bool> robotoccupato = {false, false};
int fineproduzione = 0;
vector<vector<string>> tipologie ((ceil(2*MAX/10.) + 1) ,vector<string>(10));
vector<vector<int>> orario ((ceil(2*MAX/10.) + 1) ,vector<int>(10));


int main(int argc, char* argv[]){

  if (argc!= 6) {
		cout << "missing command line arguments:\n";
		cout << "to run ./main distanza_a velocità_a distanza_b velocità_b port\n";
		exit(EXIT_FAILURE);
	}

  vector<int> veldis;
  for (int i = 1; i < 5; i++){
    veldis.push_back(atoi(argv[i]));
  }
  //thread sistemadigracefuldegradation{sistemadigracefuldegradation};

  thread arrivodeipezzi1{arrivodeipezzi, "a"};
	thread arrivodeipezzi2{arrivodeipezzi, "b"};

  thread manipolazionedeipezzidapartedeicobot1{manipolazionedeipezzidapartedeicobot, veldis, "a"};
  thread manipolazionedeipezzidapartedeicobot2{manipolazionedeipezzidapartedeicobot, veldis, "b"};

  thread riempimentodellescatole1{riempimentodellescatole, 0};
  thread riempimentodellescatole2{riempimentodellescatole, 1};

	arrivodeipezzi1.join();
	arrivodeipezzi2.join();

  manipolazionedeipezzidapartedeicobot1.join();
  manipolazionedeipezzidapartedeicobot2.join();

  riempimentodellescatole1.join();
  riempimentodellescatole2.join();

  for (int i = 0; i < tipologie.size(); i++){
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

  accept_loop(argv[5]);

  return 0;
}
