//
// Created by loumouli on 4/1/23.
//

#include "Banbot.hpp"
#include <stdexcept>
#include <sys/types.h>

bool server_up = true;

void	handler_sigint( int sig ) {
	(void)sig;
	server_up = false;
}

void Banbot::search_chan( string& str) {
    if (str.find("End of LIST") != string::npos) {
        throw invalid_argument("end of list");
    }
    size_t pos_hastag = str.find('#');
    if (pos_hastag == string::npos) {
        throw invalid_argument("wrong chan name");
    }
    str.erase(0, pos_hastag + 1);
    size_t pos_space = str.find(' ');
    if (pos_space == string::npos) {
        throw invalid_argument("cant find space in chan respond");
    }
    str.erase(pos_space, str.size());
	for ( vec_str_iter it = this->chan_server.begin(); it != this->chan_server.end(); it++ ) {
		if (*it == str)
			return;
	}
    this->chan_server.push_back(str);
	string msg = "JOIN #" + str + endmsg;
	ssize_t ret_val = send_msg(msg);
	if (ret_val == -1) {
		throw runtime_error(string("send: ") + strerror(errno));
	}
	clear_resize(msg);
	ret_val = recv_msg(msg);
	if (ret_val == -1) {
		throw runtime_error(string("recv: ") + strerror(errno));
	}
	cout << YELLOW << "joined [" << str << "]" << RESET << endl;
}

void Banbot::parse_recv_msg( string& str ) {
    while(true) {
        size_t pos = str.find(endmsg);
        if (pos != string::npos) {
            string temp = str.substr(0, pos);
            try {
                search_chan(temp);
            }
            catch ( invalid_argument& x) {
                return ;
            }
            str.erase(0, pos + 2);
        }
        else
            break ;
    }
}

void Banbot::list_chan() {
	string msg = string("LIST ") + endmsg;
	ssize_t ret_val = send_msg(msg);
	if (ret_val == -11) {
		throw runtime_error(string("send: ") + strerror(errno));
	}
	clear_resize(msg);
	ret_val = recv_msg(msg);
	if (ret_val == -1) {
		throw runtime_error(string("recv: ") + strerror(errno));
	}
	msg.resize(ret_val);
	parse_recv_msg(msg);
}

void Banbot::search_word( string& msg ) {
	string user = msg.substr(msg.find(':'), msg.find('!'));
	ssize_t hastag_pos = msg.find('#');
	ssize_t end = 0;
	while(msg[hastag_pos + end] != ' ') {
		end++;
	}
	string chan = msg.substr(hastag_pos, end);
	for ( vec_str_iter it = this->ban_word.begin(); it != this->ban_word.end(); it++ ) {
		string tmp = *it;
		if ( msg.find("PRIVMSG") != string::npos && msg.find(tmp) != string::npos) {
			string msg = "KICK " + chan + " " + chan + " :" + user;
			ssize_t ret_val = send_msg(msg);
			if (ret_val == -1) {}
			throw runtime_error()
		}
	}
}

void Banbot::check_all_chan() {
	time_t	old_time = time(NULL);
	//check for 20 second every chan
	while ( server_up && time(0) < old_time + 20) {
		string msg;
		//receive data in a non-blocking way
		clear_resize(msg);
		ssize_t ret_val = recv_msg_nonblock(msg);
		if (ret_val == -1) {
			//check if fail is due to blocking operation
			if (errno == EAGAIN)
				continue;
			throw runtime_error(string("recv: ") + strerror(errno));
		}
		if (ret_val == 0)
			continue;
		msg.resize(ret_val);
		unsigned long pos;
		while ((pos = msg.find(endmsg)) != string::npos) {
			string buff = msg.substr(0, pos);
			search_word(buff);
			msg.erase(0, pos + 1);
		}
	}
}


//TODO:
//-implement actual kick if ban word found
//-update vector of chan in another thread
void Banbot::routine() {
    cout << GREEN << "starting the bot routine" << RESET << endl;
	//clean handling of signal
	signal(SIGINT, handler_sigint);
//	signal(SIGQUIT, signal_handler);

    string msg;
    while(server_up) {
		//list all chan in server
		list_chan();
		if (this->chan_server.empty())
			continue ;
		//intercept message and check forbidden word in it
		check_all_chan();
	}
	//tell the server we're quitting
	msg = "QUIT TEST\r\n";
	send_msg(msg);
	clear_resize(msg);
	recv_msg(msg);
}