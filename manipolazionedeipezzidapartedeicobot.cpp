#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <math.h>
#include <mutex>
#include "manipolazionedeipezzidapartedeicobot.h"
using namespace std;

/*RW::RW()
:data_{0}, readers_{0}, writers_{0}
{}

// operation StartRead
//	if writers != 0 or not empty (OKtoWrite)
//		waitC(OKtoRead)
//	readers = readers + 1
//	signalC(OKtoRead)
void RW::start_read()
{
	unique_lock<mutex> mlock(mutex_);
	// must use while for spourious wake ups
	while ((writers_ != 0) || (waiting_writers_ > 0)) {
		waiting_readers_++;
		OK_to_read_.wait(mlock);
		waiting_readers_--;
		// if writers_==0 the reader was unblocked by
		// a notify_all() and need to get out of the while
		// without checking the waiting_writers_>0
		// otherwise readers get stacked till all the writers
		// are gone
		if (writers_ == 0)
			break;
	}
	readers_++;
}


//operation EndRead
//	readers = readers - 1
//	if readers = 0
//		signalC(OKtoWrite)
void RW::end_read()
{
	unique_lock<mutex> mlock(mutex_);
	readers_--;
	if (readers_ == 0)
		OK_to_write_.notify_one();

}


//operation StartWrite
//	if writers != 0 or readers != 0
//		waitC(OKtoWrite)
//	writers = writers + 1
void RW::start_write()
{
	unique_lock<mutex> mlock(mutex_);
	while ( (writers_ != 0) || (readers_ != 0) ) {
		waiting_writers_++;
		OK_to_write_.wait(mlock);
		waiting_writers_--;
	}
	writers_++;
}


//operation EndWrite
//	writers = writers - 1
//	if (empty(OKtoRead))
//	then signalC(OKtoWrite)
//	else signalC(OKtoRead)
void RW::end_write()
{
	unique_lock<mutex> mlock(mutex_);
	writers_--;
	if (waiting_readers_ > 0)
		OK_to_read_.notify_all();
	else
		OK_to_write_.notify_one();
}
*/

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
