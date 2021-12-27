#include <vector>
#include <iostream>
#include <fstream>
#include <string>
using namespace std;

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
	myfile.close();
  for (int i = 0; i < lines.size(); i++){
    size_t found1 = lines[i].find_first_of("ABC");
    size_t found2 = lines[i].find_first_of("123456789",found1);
    string pos;
    if(found2 != string::npos){
      pos = lines[i].substr(found2, string::npos);
    }else{
      pos = lines[i][string::npos-1];
    }
    cout << "Cobot linea di trasporto " << nome << ": recuperato componente di tipo " << lines[i][found1] << " al tempo MM SS in posizione " << pos << '\n';
  }



}
