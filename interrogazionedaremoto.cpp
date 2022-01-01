#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include <iostream>
#include <vector>
#include <thread>
using std::thread;
using std::cerr;
using std::cout;
using std::endl;
using std::vector;

const int MAXDATASIZE{100}; // max number of bytes we can get at once

void new_connection (int sock) {
  if (send(sock, "Di quale scatola si vogliono sapere le informazioni?", 52, 0) == -1){
    cerr << "send  \n";
  }

  char buf[MAXDATASIZE];
  int numbytes;

  if ((numbytes = recv(sock, buf, MAXDATASIZE-1, 0)) == -1) {
      cerr << "Server failed to receive data\n";
      exit(EXIT_FAILURE);
  }

  buf[numbytes] = '\0';

  cout << "server: received "<< buf << endl;

  close(sock);
}


// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int make_accept_sock (const char* port) {

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints) );
    hints.ai_family   = AF_UNSPEC; // PF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags    = AI_PASSIVE; // use my IP

    struct addrinfo* servinfo;
    int rv = getaddrinfo(NULL, port, &hints, &servinfo);
    if (rv != 0) {
        cerr << "getaddrinfo:" << gai_strerror(rv) << endl;
        exit(EXIT_FAILURE);
    }

    struct addrinfo  *p;
    int sockfd; int yes{1};
    // loop through all the results and bind to the first we can
		for(p = servinfo; p != NULL; p = p->ai_next) {

        // create a socket
        // socket(int domain, int type, int protocol)
			if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
				cerr << "server: failed to create socket\n";
				continue;
			}

			if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
				cerr << "server: failed setsockopt\n";
				exit(EXIT_FAILURE);
			}

      // bind(int fd, struct sockaddr *local_addr, socklen_t addr_length)
      // bind() passes file descriptor, the address structure,
      // and the length of the address structure
      // This bind() call will bind the socket to the current IP address
      // on port
			if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
				close(sockfd);
				cerr << "server: failed bind\n";
				continue;
			}

			break;
		}

		if (p == NULL)  {
			cerr << "server: failed to bind\n";
			exit(EXIT_FAILURE);
		}

		freeaddrinfo(servinfo); // all done with this structure


  	return sockfd;

}


void accept_loop (const char* port) {
    // create the accept socket
    int sock = make_accept_sock(port);

    while(true) {


    	// This listen() call tells the socekt to listen to the incoming
			// connections.
	    // The listen() function places all incoming connection into a backlog
			// queue until accept() call accepts the connection
  	  // Here we set the maximum size for the backlog queue to 10.

    	const int BACKLOG{10};	 // how many pending connections queue will hold
			if (listen(sock, BACKLOG) == -1) {
				cerr << "server: failed to listen\n";
	    	exit(EXIT_FAILURE);
			}
      // the accept() call actually accepts an incoming connection
      // This accept() function will write the connecting client's address info
      // into the the address structure and the size of that structure is clilen.
      // The accept() returns a new socket file descriptor for the accepted connection.
      // So, the original socket file descriptor can continue to be used
      // for accepting new connections while the new socker file descriptor is used for
      // communicating with the connected client.
      int new_sock = accept(sock, 0, 0);
      if (new_sock < 0) {
          cerr << "server: failed on accept\n";
          exit(EXIT_FAILURE);
      }
      std::thread t(new_connection, new_sock);
      t.detach();
    }
}
