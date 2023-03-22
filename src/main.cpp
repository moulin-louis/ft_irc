#include <iostream>
#include "irc.hpp"
#include "Server.hpp"
#include "Client.hpp"

int main( int ac, char **av ) {

	try
	{
		if (ac != 3) throw invalid_argument("Usage: ./ircserv <port> <password>");

		check_port(av[1]);
		Server server(av[1], av[2]);
		Socket sock = socket( AF_INET , SOCK_STREAM  , 0);
		if ( sock == -1 ) { throw runtime_error(string("socket: ") + strerror(errno)); }

		cout << "Socket created" << endl;
		sockaddr_in	sin;
		sin.sin_addr.s_addr = htonl(INADDR_ANY);
		sin.sin_family = AF_INET;
		sin.sin_port = htons(server.getPort());
		if ( bind(sock, (sockaddr *)&sin, sizeof(sin)) == -1 ) { close(sock); throw runtime_error(string("bind: ") + strerror(errno)); }

		cout << "bind done" << endl;
		if (listen(sock, 4096) == -1 ) { close(sock); throw runtime_error(string("listend: ") + strerror(errno)); }

		cout << "socket is listening.." << endl;
		while(1) {
			sockaddr csin;
			socklen_t crecsize = sizeof(csin);
			Socket csock = accept(sock, &csin, &crecsize);
			if ( csock == -1 ) { close(sock); throw runtime_error(string("accept: ") + strerror(errno)); }
			server.fd_map.insert( pair<Socket, Client>(csock, Client()) );
		}
		close(sock);
	}
	catch ( exception& x )	{
		cout << RED << x.what() << RESET << endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
