#ifndef GLOBALI
#define GLOBALI

#define MAX 3600

using std::mutex;
using std::condition_variable;
using std::vector;
using std::string;


extern mutex mutex_;
extern mutex mutexcout_;
extern mutex mutexletturafile_;
extern mutex mutexnum_;
extern mutex mutexrobot_;
extern mutex mutexpostazioneoccupata_;
extern mutex mutexfineproduzione_;
extern mutex mutexmagazzino_;
extern condition_variable not_empty_;
extern vector<condition_variable> robot_;
extern condition_variable postazioneoccupata_;
extern int num;
extern int pezziinnum;
extern bool postazioneoccupata;
extern vector<bool> robotoccupato;
extern int fineproduzione;
extern vector<vector<string>> tipologie;
extern vector<vector<int>> orario;

#endif
