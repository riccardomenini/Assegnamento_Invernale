#include <fstream>
#include <iostream>
#include <ctime>
#include <string>
#include <thread>
#include <chrono>
#include "globali.h"
using std::ofstream;
using std::endl;
using std::string;
using std::mutex;
using std::condition_variable;
using std::cout;
using std::unique_lock;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;

void arrivodeipezzi(string nome){
  ofstream fout("arrivi_linea_"+ nome +".txt"); //apertura file
  int mm = 0;
  int ss = 0;
  srand(time(NULL));

  for (int i = 0; i < MAX; i++){
    if (stato){ //controlla che non sia stato lanciato il segnale di spegnimento

    	int tipo = rand()%3; //la generazione dei pezzi è random
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

      sleep_for(milliseconds{10});  //Assunto come tempo della generazione (1 s = 10 ms) altrimenti occore aspettare troppo

      unique_lock<mutex> mlockfout(mutexfout_);                               //scrittura dei pezzi generati su file. Essendoci due thread per ogni file
      fout << mm << "\t" << ss << "\t" << tipo_pezzo << "\t" << pos << endl;  //una che scrive e una che legge occorre proteggere questa parte perchè se
      mlockfout.unlock();                                                     //fout non è thread safe la thread di lettura potrebbe leggere una riga
                                                                              //che non è ancora stata scritta del tutto

      not_empty_.notify_one();  //segnala alla thread che legge il file che sul file ci sono linee nuove

      if (ss == 59) { //serve per far scorrere il tempo
        ss = 0;
        mm ++;
      }else{
        ss ++;
      }

    }else{  //se lo stato è false il sitema viene spento e quindi viene fermata la scrittura
      break;
    }
  }
  fout.close(); //chiusura file

}
