//
// Created by loumouli on 4/1/23.
//

#ifndef FT_IRC_BANBOT_HPP
#define FT_IRC_BANBOT_HPP

#include <cstdlib>
#include <cstdlib>
#include <iostream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdlib>
#include <unistd.h>
#include <exception>
#include <fstream>
#include <cerrno>
#include <cstring>
#include "colors.h"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>
#include <csignal>
#include <ctime>

using namespace std;

#define endmsg "\r\n"

#define send_msg(msg) send(this->sfd, (void *)msg.c_str(), msg.size(), 0)
#define recv_msg(msg) recv(this->sfd, (void *)msg.c_str(), 512, 0)
#define recv_msg_nonblock(msg) recv(this->sfd, (void *)msg.c_str(), 512, MSG_DONTWAIT)
#define clear_resize(msg) msg.clear(); msg.resize(512)

class Banbot {
	public:
		int 			sfd;
		string 			admin_pasword;
		string 			serv_pass;
		string 			bot_name;
		vector<string>	ban_word;
        vector<string>  chan_server;
		sockaddr_in		sin;
		Banbot();
		~Banbot();

		//parsing
		void 	parse_conf_file();
		void 	parse_port( string& );
		void 	parse_address( string& );
		void	parse_admin_pass( string& );
		void	parse_botname( string& );
		void 	parse_password( string& );
		void	parse_banfile( );

		//connection
		void	initial_connection();

		//routine
		void	routine();
		void	list_chan();
        void	parse_recv_msg( string& );
        void	search_chan( string& );
		void	join_all_chan();
		void	check_all_chan();

};

#endif //FT_IRC_BANBOT_HPP
