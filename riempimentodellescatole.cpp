#include <vector>
#include <mutex>
#include <iostream>
#include <thread>
#include <chrono>
#include "globali.h"
using std::cout;
using std::mutex;
using std::condition_variable;
using std::endl;
using std::unique_lock;
using std::this_thread::sleep_for;
using std::chrono::milliseconds;

void riempimentodellescatole(int id){
  while (fineproduzione < 2){ //controllo che la produzione non sia finita

    unique_lock<mutex> mlockpostazioneoccupata(mutexpostazioneoccupata_);
    while (!postazioneoccupata && fineproduzione < 2){ //se viene svegliato e la postazione è occupata ma è finita la produzione smette di aspettare
      postazioneoccupata_.wait(mlockpostazioneoccupata);
    }
    mlockpostazioneoccupata.unlock();

    sleep_for(milliseconds{100}); //robot mobile: trasporto da cobot a magazzino

    unique_lock<mutex> mlockcout(mutexcout_); //il cout non è thread safe e quindi viene protetto
    if (postazioneoccupata){  //serve per la fine della produzione: vengono sbloccati entrambi i robot e bisogna far si che non portino due volte la scatola a magazzino
      cout << "Robot mobile: " << id << " recuperato la scatola " << num << " e portata in magazzino" << endl;
    }
    mlockpostazioneoccupata.lock();//modifico una variabile globale quindi proteggo con un semaforo
    postazioneoccupata = false; //è qui in quanto a volte ottenevo in output che l'ultima scatola veniva portata a magazzino dai robot 2 volte e ho
                                //supposto che accadeva perchè le thread venivano eseguite entrambe fino al cout e prima di questa istruzione, per
                                //cui mettendole in questa sezione critica questo non dovrebbe accadere
    mlockpostazioneoccupata.unlock();
    mlockcout.unlock();

    postazioneoccupata_.notify_all();//la postazione non è più occupata quindi sveglio chi si era addormentato su questa variabile condizione

  }

}
