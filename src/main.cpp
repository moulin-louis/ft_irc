#include <iostream>
#include "irc.hpp"

int main( int ac, char **av ) {
	if ( ac != 3 ) {
		cout << "usage is : ./ircserv <port> <password>";
		return EXIT_FAILURE;
	}
	int port = atoi(av[1]);
	
	Socket sock = socket( AF_INET, SOCK_STREAM  , 0);
	if (sock == -1) {
		cout << "socket creatin: " << strerror(errno) << endl;
		return EXIT_FAILURE;
	}

	cout << "Socket created" << endl;

	sockaddr_in	sin;
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);

	if (bind(sock, (sockaddr *)&sin, sizeof(sin)) == -1 ) {
		perror("bind: ");
		close(sock);
		return EXIT_FAILURE;
	}

	cout << "bind done" << endl;

	if (listen(sock, 4096) == -1 ) {
		perror("listen: ");
		close(sock);
		return EXIT_FAILURE;
	}

	cout << "socket is listening.." << endl;

	sockaddr_in csin;
	socklen_t crecsize = sizeof(csin);
	Socket csock = accept(sock, (sockaddr*)&csin, &crecsize);
	if (csock == -1) {
		perror("accept: ");
		close(sock);
		return EXIT_FAILURE;
	}
	cout << "connection accepted" << endl;
	if (send(csock, "Hello World\n", strlen("Hello Word") + 1, 0) == -1) {
		perror("send: ");
		close(sock);		
		close(csock);
		return EXIT_FAILURE;
	}
	close(sock);
	close(csock);
	return EXIT_SUCCESS;
}