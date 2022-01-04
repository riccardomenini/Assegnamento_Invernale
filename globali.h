#ifndef GLOBALI
#define GLOBALI

#define MAX 3600

using std::mutex;
using std::condition_variable;
using std::vector;
using std::string;

extern mutex mutexfout_;
extern mutex mutexcout_;
extern mutex mutexpostazioneoccupata_;
extern mutex mutexfineproduzione_;
extern mutex mutexinserimentopezzi_;
extern condition_variable not_empty_;
extern condition_variable postazioneoccupata_;
extern int num;
extern int pezziinnum;
extern bool postazioneoccupata;
extern int fineproduzione;
extern vector<vector<string>> tipologie;
extern vector<vector<int>> orario;
extern bool stato;
extern int numerosocket;
extern bool socketaperta;

#endif
