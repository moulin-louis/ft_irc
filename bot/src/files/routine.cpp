//
// Created by loumouli on 4/1/23.
//

#include "Banbot.hpp"

bool server_up = true;

pthread_mutex_t lock;

void handler_sigint(int sig) {
	(void) sig;
	cout << "sigint in thread" << endl;
	server_up = false;
}

bool check_server_up() {
	pthread_mutex_lock(&lock);
	if (server_up) {
		pthread_mutex_unlock(&lock);
		return true;
	}
	pthread_mutex_unlock(&lock);
	return false;
}

void Banbot::search_chan(string &str) {
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
		if (*it == str) {
			return;
		}
	}
    this->chan_server.push_back(str);
	string msg = "JOIN #" + str + endmsg;
	pthread_mutex_lock(&(this->lock_socket));
	ssize_t ret_val = send_msg(msg);
	if (ret_val == -1) {
		pthread_mutex_unlock(&(this->lock_socket));
		throw runtime_error(string("send: ") + strerror(errno));
	}
	clear_resize(msg);
	ret_val = recv_msg(msg);
	if (ret_val == -1) {
		pthread_mutex_unlock(&(this->lock_socket));
		throw runtime_error(string("recv: ") + strerror(errno));
	}
	pthread_mutex_unlock(&(this->lock_socket));
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

void *list_chan( void*ptr ) {
	Banbot	*bot = (Banbot *)ptr;
	while (check_server_up()) {
		string msg = string("LIST ") + endmsg;
		pthread_mutex_lock(&(bot->lock_socket));
		ssize_t ret_val = send_msg_bot(msg);
		if (ret_val == -1) {
			pthread_mutex_unlock(&(bot->lock_socket));
			throw runtime_error(string("send: ") + strerror(errno));
		}
		clear_resize(msg);
		ret_val = recv_msg_bot(msg);
		if (ret_val == -1) {
			pthread_mutex_unlock(&(bot->lock_socket));
			throw runtime_error(string("recv: ") + strerror(errno));
		}
		pthread_mutex_unlock(&(bot->lock_socket));
		msg.resize(ret_val);
		bot->parse_recv_msg(msg);
	}
	return (NULL);
}

void Banbot::search_word( string& msg ) {
	string user = msg.substr(msg.find(':') + 1, msg.find('!') - 1);
	size_t hastag_pos = msg.find('#') + 1;
	size_t end = 0;
	while(msg[hastag_pos + end] != ' ') {
		end++;
	}
	string chan = msg.substr(hastag_pos, end);
	for ( vec_str_iter it = this->ban_word.begin(); it != this->ban_word.end(); it++ ) {
		string tmp = *it;
		if ( msg.find("PRIVMSG") != string::npos && msg.find(tmp) != string::npos) {
			string msg_to_send = "KICK #";
			msg_to_send += chan + " ";
			msg_to_send += chan + " :";
			msg_to_send += user  + endmsg;
			pthread_mutex_lock(&(this->lock_socket));
			ssize_t ret_val = send_msg(msg_to_send);
			if (ret_val == -1) {
				pthread_mutex_unlock(&(this->lock_socket));
				throw runtime_error(string("send: ") + strerror(errno));
			}
			pthread_mutex_unlock(&(this->lock_socket));
		}
	}
}

void Banbot::check_all_chan() {
	while (check_server_up()) {
		string msg;
		//receive data in a non-blocking way
		clear_resize(msg);
		pthread_mutex_lock(&(this->lock_socket));
		ssize_t ret_val = recv_msg_nonblock(msg);
		if (ret_val == -1) {
			//check if fail is due to blocking operation
			pthread_mutex_unlock(&(this->lock_socket));
			if (errno == EAGAIN)
				continue;
			throw runtime_error(string("recv: ") + strerror(errno));
		}
		if (ret_val == 0)
			continue;
		pthread_mutex_unlock(&(this->lock_socket));
		msg.resize(ret_val);
		unsigned long pos;
		while ((pos = msg.find(endmsg)) != string::npos) {
			string buff = msg.substr(0, pos);
			search_word(buff);
			msg.erase(0, pos + 1);
		}
	}
}

void Banbot::routine() {
    cout << GREEN << "starting the bot routine" << RESET << endl;
	signal(SIGINT, handler_sigint);
	pthread_mutex_init(&lock, NULL);
	pthread_create(&(this->id_thread), NULL, &list_chan, (void *) this);
    string msg;
    while(server_up) {
		check_all_chan();
	}
	pthread_join(this->id_thread, NULL);
	pthread_mutex_destroy(&lock);
	msg = "QUIT TEST\r\n";
	send_msg(msg);
	clear_resize(msg);
	recv_msg(msg);
}