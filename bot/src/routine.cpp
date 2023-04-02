//
// Created by loumouli on 4/1/23.
//

#include "Banbot.hpp"
#include <ctime>

bool server_up = true;

void signal_handler( int sig ) {
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
    cout << "pushing " << str << " to chan vec" << endl;
    this->chan_server.push_back(str);
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
		throw runtime_error(string("send:") + strerror(errno));
	}
	clear_resize(msg);
	ret_val = recv_msg(msg);
	if (ret_val == -1) {
		throw runtime_error(string("recv:") + strerror(errno));
	}
	msg.resize(ret_val);
	parse_recv_msg(msg);
}

void Banbot::join_all_chan() {
	for ( vector<string>::iterator it = this->chan_server.begin(); it != this->chan_server.end(); it++ ) {
		string msg = "JOIN #" + *it;
		ssize_t ret_val = send_msg(msg);
		if (ret_val == -1) {
			throw runtime_error(string("send:") + strerror(errno));
		}
		clear_resize(msg);
		ret_val = recv_msg(msg);
		if (ret_val == -1) {
			throw runtime_error(string("recv:") + strerror(errno));
		}
		cout << "joined : " << *it << endl;
	}
}

void Banbot::check_all_chan() {
	time_t	old_time = time(NULL);

	//check for 20 second every chan
	while ( time(0) < old_time + 20 || server_up) {
		string msg;
		recv_msg(msg);
		for ( vector<string>::iterator it = this->ban_word.begin(); it != this->ban_word.end(); it++ ) {
			if ( msg.find(*it) != string::npos) {
				//banword found
				cout << "banword found" << endl;
			}
		}
	}
}

void Banbot::routine() {
    cout << GREEN << "starting the bot routine" << RESET << endl;

	//clean handling of signal
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);

    string msg;
	list_chan();
    while(server_up) {
		//list all chan in server
		list_chan();
		//join this chan
        join_all_chan();
		//intercept message and check forbidden word in it
		check_all_chan();
	}

	//tell the server were quitting
	msg = "QUIT TEST\r\n";
	cout << "sending data..." << endl;
	send_msg(msg);
	clear_resize(msg);
	recv_msg(msg);
}