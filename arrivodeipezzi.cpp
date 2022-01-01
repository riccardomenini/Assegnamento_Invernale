#include <fstream>
#include <iostream>
#include <ctime>
#include <string>
#include "globali.h"
using std::ofstream;
using std::endl;
using std::string;
using std::mutex;
using std::condition_variable;
using std::cout;
using std::unique_lock;

void arrivodeipezzi(string nome){
  ofstream fout("arrivi_linea_"+ nome +".txt");
  int mm = 0;
  int ss = 0;
  srand(time(NULL));
  for (int i = 0; i < MAX; i++){
		int tipo = rand()%3;
    string tipo_pezzo;
    switch (tipo) {
      case 0:
        tipo_pezzo = "A";
        break;
      case 1:
        tipo_pezzo = "B";
        break;
      case 2:
        tipo_pezzo = "C";
        break;
    }
    int pos = rand()%1000;
    unique_lock<mutex> mlock(mutex_);
    fout << mm << "\t" << ss << "\t" << tipo_pezzo << "\t" << pos << endl;
    mlock.unlock();
		not_empty_.notify_one();
    if (ss == 59) {
      ss = 0;
      mm ++;
    }else{
      ss ++;
    }
  }
  fout.close();

}
