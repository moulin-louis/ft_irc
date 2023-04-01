//
// Created by loumouli on 4/1/23.
//

#ifndef FT_IRC_BANBOT_HPP
#define FT_IRC_BANBOT_HPP

#include <stdlib.h>
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
#include <errno.h>
#include <cstring>
#include "colors.h"
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <vector>
#include <sys/types.h>
#include <sys/socket.h>

using namespace std;

#define endmsg "\r\n"
#define send_msg(msg) send(this->sfd, (void *)msg.c_str(), msg.size(), 0)

class Banbot {
	public:
		int 			sfd;
		string 			admin_pasword;
		string 			serv_pass;
		string 			bot_name;
		vector<string>	ban_word;
		sockaddr_in		sin;
		Banbot();
		~Banbot();

		//parsing
		void parse_conf_file();
		void parse_port( string& );
		void parse_address( string& );
		void parse_admin_pass( string& );
		void parse_botname( string& );
		void parse_password( string& );

		//connection
		void initial_connection();

		//routine
		void routine();

		//clean/quit
		void	clean_n_quit();

};

#endif //FT_IRC_BANBOT_HPP
