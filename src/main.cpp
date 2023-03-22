#include "irc.hpp"
#include "Server.hpp"
#include "Client.hpp"



int main( int ac, char **av )
{
	try
	{
		if (ac != 3) throw invalid_argument("Usage: ./ircserv <port> <password>");
		check_port(av[1]);
		Server server(av[1], av[2]);
//		while (true)
//		{
//			sockaddr csin = {};
//			socklen_t crecsize = sizeof(csin);
//			Socket csock = accept(server._sfd, &csin, &crecsize);
//			if ( csock == -1 ) { close(server._sfd); throw runtime_error(string("accept: ") + strerror(errno)); }
//			server.fd_map.insert( pair<Socket, Client>(csock, Client()) );
//		}
		server.run();
	}
	catch ( exception& x )
	{
		cout << RED << x.what() << RESET << endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
