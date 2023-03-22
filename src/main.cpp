#include <iostream>
#include "irc.hpp"
#include "Server.hpp"

int main( int ac, char **av ) {
	
	// Test instantiation of Server class
	Server server(av[1], av[2]);
	try {
		if ( ac != 3 ) {
			throw invalid_argument("pas bien args");
	}
		Socket sock = socket( AF_INET , SOCK_STREAM  , 0);
		if ( sock == -1 ) {
			throw runtime_error(string("socket: ") + strerror(errno));
		}
		cout << "Socket created" << endl;
		sockaddr_in	sin;
		sin.sin_addr.s_addr = htonl(INADDR_ANY);
		sin.sin_family = AF_INET;
		sin.sin_port = htons(server.getPort_uint());	
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
			buf[len_recv] = '\0';
			cout << len_recv << " received" << endl;
			buf.resize(len_recv + 1);
			while (buf.find('\r') != string::npos) {
				buf.replace(buf.find('\r'), 1, "");
			}
			cout << buf << endl;
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
		cout << x.what() << endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}