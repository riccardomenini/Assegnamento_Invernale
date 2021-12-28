#include <thread>
#include <math.h>
#include <iostream>
#include <vector>
#include "arrivodeipezzi.h"
#include "manipolazionedeipezzidapartedeicobot.h"
using std::vector;
using std::thread;
using std::cout;

int main(int argc, char* argv[]){

  if (argc!= 5) {
		cout << "missing command line arguments:\n";
		cout << "to run ./main distanza_a velocità_a distanza_b velocità_b\n";
		exit(EXIT_FAILURE);
	}

  vector<int> veldis;
  for (int i = 1; i < 5; i++){
    veldis.push_back(atoi(argv[i]));
  }

  thread arrivodeipezzi1{arrivodeipezzi, "a"};
	thread arrivodeipezzi2{arrivodeipezzi, "b"};

  //thread manipolazionedeipezzidapartedeicobot1{manipolazionedeipezzidapartedeicobot, veldis, "a"};
  //thread manipolazionedeipezzidapartedeicobot2{manipolazionedeipezzidapartedeicobot, veldis, "b"};


	arrivodeipezzi1.join();
	arrivodeipezzi2.join();

  thread manipolazionedeipezzidapartedeicobot1{manipolazionedeipezzidapartedeicobot, veldis, "a"};

  manipolazionedeipezzidapartedeicobot1.join();
  //manipolazionedeipezzidapartedeicobot2.join();


	return 0;
}
