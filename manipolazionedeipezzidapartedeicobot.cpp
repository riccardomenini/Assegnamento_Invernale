#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <mutex>
#include "manipolazionedeipezzidapartedeicobot.h"
using std::string;
using std::vector;
using std::ifstream;
using std::cout;

void manipolazionedeipezzidapartedeicobot(const vector<int> & veldis, string nome){
	ifstream myfile("arrivi_linea_"+ nome +".txt");
	if (!myfile.is_open()) {
		cout << "unable to open file: " << "arrivi_linea_"+ nome +".txt" << "\n";
		exit(EXIT_FAILURE);
  }
	vector<string> lines;
	string a_line;
	while (getline (myfile, a_line))
		lines.push_back(a_line);

  for (int i = 0; i < lines.size(); i++){
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
      mm += floor((veldis[0]/veldis[1])/60.);
      ss += (veldis[0]/veldis[1])%60;
    }else{
      mm += floor((veldis[2]/veldis[3])/60.);
      ss += (veldis[2]/veldis[3])%60;
    }
    cout << "Cobot linea di trasporto " << nome << ": recuperato componente di tipo " << lines[i][found1] << " al tempo " << mm << " " << ss << " in posizione " << pos << '\n';
  }
  myfile.close();



}
