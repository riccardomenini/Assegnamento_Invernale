#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <mutex>
#include "globali.h"
using std::string;
using std::vector;
using std::ifstream;
using std::cout;
using std::mutex;
using std::condition_variable;
using std::endl;
using std::unique_lock;

void manipolazionedeipezzidapartedeicobot(const vector<int> & veldis, string nome){
	vector<string> lines;
	string a_line;
	int lineelette;
	while(lineelette < 3600){
    std::unique_lock<mutex> mlock(mutex_);
  	while (lines.size() == lineelette){
      not_empty_.wait(mlock);
			ifstream myfile("arrivi_linea_"+ nome +".txt");
			if (!myfile.is_open()) {
				unique_lock<mutex> mlock(mutex_);
				cout << "unable to open file: " << "arrivi_linea_"+ nome +".txt" << "\n";
				mlock.unlock();
				exit(EXIT_FAILURE);
		  }
			lines = {};
			while (getline (myfile, a_line)){
				lines.push_back(a_line);
			}
			myfile.close();
    }
    for (int i = lineelette; i < lines.size(); i++){
      size_t found1 = lines[i].find_first_of("ABC");
      size_t found2 = lines[i].find_first_of("0123456789",found1);
      size_t found3 = lines[i].find_first_of("0123456789");
      size_t found4 = lines[i].find_first_not_of("0123456789");
      size_t found5 = lines[i].find_first_of("0123456789",found4);
      size_t found6 = lines[i].find_first_not_of("0123456789",found5) - 2;
      int mm = stoi(lines[i].substr(found3, found4));
      int ss = stoi(lines[i].substr(found5, found6));
      string pos;
      pos = lines[i].substr(found2, string::npos);
      if (nome == "a"){
        //mm += floor((veldis[0]/veldis[1])/60.);
        ss += (veldis[0]/veldis[1]) + mm * 60;
				mm = floor(ss/60.);
				ss = ss%60;
      }else{
				ss += (veldis[0]/veldis[1]) + mm * 60;
				mm = floor(ss/60.);
				ss = ss%60;
      }
			unique_lock<mutex> mlock(mutexcout_);
      cout << "Cobot linea di trasporto " << nome << ": recuperato componente di tipo " << lines[i][found1] << " al tempo " << mm << " " << ss << " in posizione " << pos << '\n';
			mlock.unlock();
		}
		lineelette=lines.size();
  }
}
