
#include "../inc/irc.hpp"
 
int main( int ac, char **av )
{
	if ( ac != 2 ) {
		cout << "usage is : ./ircserv <port> ";
		return EXIT_FAILURE;
	}
	int port = atoi(av[1]);
    Socket sock;
    sockaddr_in sin;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	sin.sin_addr.s_addr = inet_addr("127.0.0.1");
	sin.sin_family = AF_INET;
	sin.sin_port = htons(port);
 
	if(connect(sock, (sockaddr*)&sin, sizeof(sin)) != SOCKET_ERROR)
	{
		cout << "connected to " << inet_ntoa(sin.sin_addr) << " on port " << htons(sin.sin_port) << endl;
		cout << "im here" << endl;
		string buf = "NICK llr";
		cout << "sending data..." << endl;
		send(sock, (void *)buf.c_str(), buf.size(), 0);
		buf = "USER loumouli";
		cout << "sending data..." << endl;
		if (send(sock, (void *)buf.c_str(), buf.size(), 0) == -1) {
			cout << "send failed " << 	strerror(errno) << endl;
			return 1;
		}
		buf.resize(500);
		cout << "receiving data" << endl;
		int len_recv = recv(sock, (void *)(buf.c_str()), 500, 0);
		if (len_recv == -1) {
			cout << "receive failed" << endl;
			close(sock);
			return EXIT_FAILURE;
		}
		cout << buf << endl;
	}
	else
		cout << "Cant connect" << endl;
	close(sock);
	cout << "socket close" << endl;
    return EXIT_SUCCESS;
}	