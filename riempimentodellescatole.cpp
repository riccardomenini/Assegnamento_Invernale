#include <vector>
#include <mutex>
#include <iostream>
#include "globali.h"
using std::cout;
using std::mutex;
using std::condition_variable;
using std::endl;
using std::unique_lock;

void riempimentodellescatole(int id){
  while (fineproduzione < 2){

    unique_lock<mutex> mlockrobot(mutexrobot_);
    while(robotoccupato[id]){
      robot_[id].wait(mlockrobot);
    }

    unique_lock<mutex> mlockpostazioneoccupata(mutexpostazioneoccupata_);
    while (!postazioneoccupata && fineproduzione < 2){
      postazioneoccupata_.wait(mlockpostazioneoccupata);
    }
    mlockpostazioneoccupata.unlock();


    robotoccupato[id] = true;

    unique_lock<mutex> mlockcout(mutexcout_);
    if (postazioneoccupata){//serve per la fine della produione per sbloccare entrambi i robot
      cout << "Robot mobile: " << id << " recuperato la scatola " << num << " e portata in magazzino" << endl;
    }
    mlockcout.unlock();

    mlockpostazioneoccupata.lock();
    postazioneoccupata = false;
    mlockpostazioneoccupata.unlock();

    postazioneoccupata_.notify_all();

    robotoccupato[id] = false;

    /*unique_lock<mutex> mlockscatoleportate(mutexscatoleportate_);
    cout << "Aumento le scatole: " << endl;
    scatoleportate ++;
    mlockscatoleportate.unlock();*/

  }

}
