#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream>
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
#include "globali.h"
#include <csignal>
using std::cout;
using std::endl;
using std::unique_lock;

void signal_handler(int signal){
  
  unique_lock<mutex> mlockcout(mutexcout_); //il cout non è thread safe e quindi viene protetto
  cout << "Spegnimento impianto..." << endl;
  mlockcout.unlock();

  stato = false;  //segnalo a tutti che il programma viene spento

  if (socketaperta){  //controllo se la socket è aperta e in tal caso la chiudo
    close(numerosocket);
  }

}

void sistemadigracefuldegradation(){

  std::signal(SIGINT, signal_handler);  //funzione che raccoglie il segnale

}
