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
#include <sstream>
#include "curl.h"
using namespace std;

#define endmsg "\r\n"
#define SIZE_BUFFER 20000
#define CHUNK_SIZE 426
#define send_msg(msg) send(this->sfd, (void *)msg.c_str(), msg.size(), 0)
#define send_msg_bot(msg) send(bot->sfd, (void *)msg.c_str(), msg.size(), 0)
#define recv_msg(msg) recv(this->sfd, (void *)msg.c_str(), SIZE_BUFFER, 0)
#define recv_msg_bot(msg) recv(bot->sfd, (void *)msg.c_str(), SIZE_BUFFER, 0)
#define recv_msg_nonblock(msg) recv(this->sfd, (void *)msg.c_str(), SIZE_BUFFER, MSG_DONTWAIT)
#define clear_resize(msg) msg.clear(); msg.resize(SIZE_BUFFER)
#define	vec_str_iter vector<string>::iterator

typedef struct s_MemoryStruct
{
	char *memory;
	size_t size;
}   t_ms;

class Banbot {
    private:
        string  _api;
    public:
		int 			sfd;
		string 			admin_pasword;
		string 			serv_pass;
		string 			bot_nickname;
		string 			bot_username;
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
        void    parse_api( );
		void	parse_banfile( );

		//connection
		void	initial_connection();

		//routine
		void	routine();
		void	initial_chan_join();
		void	parse_recv_msg( string& str );
		void	search_chan(string &str) const;
		void	check_all_chan();
		int	search_word( string& );
		string	chatgpt(string const &str);

        //getter
        string  getApi();
};

#endif //FT_IRC_BANBOT_HPP
