#include <iostream>
#include "irc.hpp"
#include "Server.hpp"

int main( int ac, char **av ) {

	try
	{
		if (ac != 3)
			throw invalid_argument("Usage: ./ircserv <port> <password>");
		check_port(av[1]);
		Server server(av[1], av[2]);
		Socket sock = socket( AF_INET , SOCK_STREAM  , 0);
		if ( sock == -1 ) {
			throw runtime_error(string("socket: ") + strerror(errno));
		}
		cout << "Socket created" << endl;
		sockaddr_in	sin;
		sin.sin_addr.s_addr = htonl(INADDR_ANY);
		sin.sin_family = AF_INET;
		sin.sin_port = htons(server.getPort());
		if ( bind(sock, (sockaddr *)&sin, sizeof(sin)) == -1 ) {
			close(sock);
			throw runtime_error(string("bind: ") + strerror(errno));
		}
		cout << "bind done" << endl;
		if (listen(sock, 4096) == -1 ) {
			close(sock);
			throw runtime_error(strerror(errno));
		}
		cout << "socket is listening.." << endl;
		while(1) {
			// struct	pollfd fds[200];
			// nfds_t	nfds = 1;
			// memset(&fds, 0, sizeof(fds));
			// fds[0].fd = sock;
			// fds[1].events = POLLOUT;
			// int rc = poll(fds, nfds, 30);
			// if ( rc == -1) {
			// 	close(sock);
			// 	throw runtime_error(strerror(errno));
			// }
			// if ( rc == 0 ) {
			// 	cout << "rc == 0, 0 connection waiting" << endl;
			// 	continue ;
			// }
			sockaddr_in csin;
			socklen_t crecsize = sizeof(csin);
			Socket csock = accept(sock, (sockaddr*)&csin, &crecsize);
			if ( csock == -1 ) {
				close(sock);
				throw runtime_error(string("accept: ") + strerror(errno));
			}
			cout << "connection accepted" << endl;
			string buf;
			buf.resize(500);
			int len_recv = recv(csock, (void *)(buf.c_str()), 500, 0);
			if (len_recv == -1) {
				close(sock);
				throw runtime_error(string("recv: ") + strerror(errno));
			}
			cout << buf << endl;
			if ( buf == "CAP LS" ) {
				cout << "buf is cap ls, should return a version" << endl;
			}
			// if ( send(csock, "Hello World\n", strlen("Hello Word") + 1, 0) == -1 ) {
			// 	close(sock);
			// 	close(csock);
			// 	throw runtime_error(string("send: ") + strerror(errno));
			// }
			// cout << "message sent" << endl;
		}
		close(sock);
	}
	catch ( exception& x )	{
		cout << RED << x.what() << RESET << endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}