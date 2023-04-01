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

using namespace std;

class Banbot {
	public:
		uint16_t	port;
		uint32_t	address;
		int 		socket;
		string 		admin_pasword;
		string 		bot_name;
		Banbot();
		~Banbot();
		void parse_conf_file();
		void parse_port( string& );
		void parse_address( string& );
};

#endif //FT_IRC_BANBOT_HPP
