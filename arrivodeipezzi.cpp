#include <fstream>
#include <iostream>
#include <ctime>
#include <string>
using namespace std;

void arrivodeipezzi(string nome){
  ofstream fout("arrivi_linea_"+ nome +".txt");
  int mm = 0;
  int ss = 0;
  srand(time(NULL));
  while (mm != 60){
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
    fout << mm << "\t" << ss << "\t" << tipo_pezzo << "\t" << pos << endl;
    if (ss == 59) {
      ss = 0;
      mm ++;
    }else{
      ss ++;
    }
  }
  fout.close();
}
