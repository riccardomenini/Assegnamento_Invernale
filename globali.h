#ifndef GLOBALI
#define GLOBALI

using std::mutex;
using std::condition_variable;

extern mutex mutex_;
extern mutex mutexcout_;
extern condition_variable not_empty_;

#endif
