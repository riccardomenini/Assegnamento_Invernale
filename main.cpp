#include <thread>
#include "arrivodeipezzi.h"
using namespace std;

int main(){
  thread arrivodeipezzi1{arrivodeipezzi, "a"};
	thread arrivodeipezzi2{arrivodeipezzi, "b"};

	arrivodeipezzi1.join();
	arrivodeipezzi2.join();

	return 0;
}
