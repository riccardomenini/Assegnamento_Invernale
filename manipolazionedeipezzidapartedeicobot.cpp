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

vector<string> leggifile(string nome, vector<string> & lines){
	string a_line;
	unique_lock<mutex> mlockletturafile(mutexletturafile_);
	ifstream myfile("arrivi_linea_"+ nome +".txt");
	if (!myfile.is_open()) {
		unique_lock<mutex> mlockcout(mutexcout_);
		cout << "unable to open file: " << "arrivi_linea_"+ nome +".txt" << "\n";
		mlockcout.unlock();
		exit(EXIT_FAILURE);
	}
	lines = {};
	while (getline (myfile, a_line)){
		lines.push_back(a_line);
	}
	myfile.close();
	mlockletturafile.unlock();
	return lines;
}

void manipolazionedeipezzidapartedeicobot(const vector<int> & veldis, string nome){
	int lineelette = 0;
	vector<string> lines;
	while(lineelette < MAX){
		leggifile(nome,lines);
		unique_lock<mutex> mlock(mutex_);
  	while (lines.size() == lineelette){
			not_empty_.wait(mlock);
			lines = leggifile(nome,lines);
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
      string pos = lines[i].substr(found2, string::npos);

      if (nome == "a"){
        ss += (veldis[0]/veldis[1]) + mm * 60;
				mm = floor(ss/60.);
				ss = ss%60;
      }else{
				ss += (veldis[0]/veldis[1]) + mm * 60;
				mm = floor(ss/60.);
				ss = ss%60;
      }

			unique_lock<mutex> mlocknum(mutexnum_);
			pezziinnum ++;
			mlocknum.unlock();

			unique_lock<mutex> mlockcout(mutexcout_);
			cout << "Cobot linea di trasporto " << nome << ": recuperato componente di tipo " << lines[i][found1] << " al tempo " << mm << " " << ss << " in posizione " << pos << " e inserito nella scatola " << num << '\n';
			lineelette = lines.size();
			mlockcout.unlock();

			unique_lock<mutex> mlockmagazzino(mutexmagazzino_);
			tipologie[num][pezziinnum - 1] = lines[i][found1];
			orario[num][pezziinnum - 1] = mm * 60 + ss;
			mlockmagazzino.unlock();

			if (pezziinnum > 9){
				mlocknum.lock();
				pezziinnum = pezziinnum % 10;
				mlocknum.unlock();

				unique_lock<mutex> mlockpostazioneoccupata(mutexpostazioneoccupata_);
				postazioneoccupata = true;
				mlockpostazioneoccupata.unlock();

				postazioneoccupata_.notify_all();

				robot_[0].notify_one();
				robot_[1].notify_one();

				mlockpostazioneoccupata.lock();
				while (postazioneoccupata){
					postazioneoccupata_.wait(mlockpostazioneoccupata);
				}

				mlocknum.lock();
				num ++;
				mlocknum.unlock();
			}
		}
  }

	unique_lock<mutex> mlockfineproduzione(mutexfineproduzione_);
	fineproduzione ++;
	if (fineproduzione == 2){//sbloccare i robot e tirare via la scatola anche se non è piena
		robot_[0].notify_one();
		robot_[1].notify_one();
		unique_lock<mutex> mlockpostazioneoccupata(mutexpostazioneoccupata_);
		postazioneoccupata = true;
		mlockpostazioneoccupata.unlock();
		postazioneoccupata_.notify_all();
	}
	mlockfineproduzione.unlock();

	//cout << "tipologie " << num << " " << pezziinnum << " " << tipologie[num][pezziinnum] << endl;
}
