/*
** client.cpp -- a stream socket client demo
*/

#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <string>
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::to_string;
using std::cin;


const int MAXDATASIZE{100}; // max number of bytes we can get at once

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{

	if (argc != 3) {
	    cerr << "usage: ./client hostname port\n";
	    exit(EXIT_FAILURE);
	}


	int sockfd, numbytes;
	char buf[MAXDATASIZE];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];


	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	if ((rv = getaddrinfo(argv[1], argv[2], &hints, &servinfo)) != 0) {
		cerr <<  "getaddrinfo: " << gai_strerror(rv) << endl;
		exit(EXIT_FAILURE);
	}

	// loop through all the results and connect to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			cerr << "Failed to create socket:\n";
			continue;
		}

		if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			cerr << "Failed to connect to socket...";
			continue;
		}

		break;
	}

	if (p == NULL) {
		cerr <<  "client: failed to connect\n";
		exit(EXIT_FAILURE);
	}

	inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
			s, sizeof(s));
	cout << "client: connecting to " << s << endl;

	freeaddrinfo(servinfo); // all done with this structure

	if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	    cerr << "Client failed to receive data\n";
	    exit(EXIT_FAILURE);
	}

	buf[numbytes] = '\0';

	cout << "client: received "<< buf << endl;

	int richiesta = 0;
	cout << "Inserire un numero: " << endl;
	cin >> richiesta;

	if (send(sockfd, "ciao", 5, 0) == -1){
    cerr << "send  \n";
  }

	close(sockfd);

	return 0;
}
