#include "irc.hpp"
#include "Server.hpp"

int main( int ac, char **av )
{
	try
	{
		if (ac != 3) throw invalid_argument("Usage: ./ircserv <port> <password>");
		check_port(av[1]);
		Server server(av[1], av[2]);
		server.run();
	}
	catch ( exception& x )
	{
		cout << RED << x.what() << RESET << endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
