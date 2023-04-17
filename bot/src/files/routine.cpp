//
// Created by loumouli on 4/1/23.
//

#include "Banbot.hpp"
#include <cstddef>
#include <cstring>
#include <algorithm>

bool server_up = true;

void handler_sigint(int sig) {
	(void) sig;
	server_up = false;
}

string requestify(string const &str) {
	string res;
	res = "{\"model\": \"gpt-3.5-turbo\", \"messages\": [{\"role\": \"user\", \"content\": \"" + str + "\"}]}";
	return (res);
}

size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
	size_t realsize = size * nmemb;
	t_ms *mem = (t_ms *) userp;

	char *ptr = (char *) realloc(mem->memory, mem->size + realsize + 1);
	if (ptr == NULL) {
		free(mem->memory);
		throw runtime_error(string("WriteMemoryCallback: enomem"));
	}
	mem->memory = ptr;
	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;
	return (realsize);
}

string	Banbot::chatgpt(string const &str) {
	CURL *curl;
	CURLcode res;
	string request;
	string api;
	t_ms chunk;
	struct curl_slist *headers;

	size_t pos = str.find("PRIVMSG");
	if (pos == string::npos)
		return ("Not a ChatGpt request");
	headers = NULL;
	chunk.memory = (char *) malloc(1);
	chunk.size = 0;
	api = "Authorization: Bearer " + this->getApi();
	request = requestify(str);
	curl = curl_easy_init();
	if (curl) {
		try {
			headers = curl_slist_append(headers, "Content-Type: application/json");
			headers = curl_slist_append(headers, api.c_str());
			curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
			curl_easy_setopt(curl, CURLOPT_URL, "https://api.openai.com/v1/chat/completions");
			curl_easy_setopt(curl, CURLOPT_POSTFIELDS, request.c_str());
			curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
			curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *) &chunk);
			res = curl_easy_perform(curl);
			if (res != CURLE_OK)
				cerr << "curl_easy_perform() failed: " << curl_easy_strerror(res) << endl;
			curl_easy_cleanup(curl);
			curl_slist_free_all(headers);
			string ret(chunk.memory);
			free(chunk.memory);
			return (ret);
		}
		catch (exception &x) {
			cout << RED << x.what() << RESET << endl;
			curl_easy_cleanup(curl);
			curl_slist_free_all(headers);
			throw runtime_error(string("chatgpt error:") + ::strerror(errno));
		}
	}
	return ("FAIL, this message should never show up");
}

void Banbot::search_chan(string &str) const {
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
}

void Banbot::parse_recv_msg(string &str) {
	while (true) {
		size_t pos = str.find(endmsg);
		if (pos != string::npos) {
			string temp = str.substr(0, pos);
			try {
				search_chan(temp);
			}
			catch (invalid_argument &x) {
				return;
			}
			str.erase(0, pos + 2);
		} else
			break;
	}
}

void Banbot::initial_chan_join() {
	string msg = string("LIST ") + endmsg;
	ssize_t ret_val = send_msg(msg);
	if (ret_val == -1) {
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

int Banbot::search_word(string &msg) {
	cout << "OG msg = [" << msg << "]" << endl;
	string saving = msg;
	size_t pos = msg.find("PRIVMSG");
	if (pos == string::npos)
		return 0;
	pos += strlen("PRIVMSG") + 1;
	msg.erase(0, pos);
	string token = msg.substr(0, msg.find(' '));
	if (token == this->bot_nickname) {
		msg = saving;
		return 0;
	}
	token.erase(0, 1);
	string content = msg.substr(msg.find(':') + 1, msg.length());
	string user = saving.substr(saving.find(':') + 1, saving.find('!') - 1);
	for ( vec_str_iter it = this->ban_word.begin(); it != this->ban_word.end(); it++ ) {
		string tmp = *it;
		if ( content.find(*it) != string::npos) {
			string msg_to_send = "KICK #";
			msg_to_send += token + " ";
			msg_to_send += token + " :";
			msg_to_send += user  + endmsg;
			cout << "sending " << msg_to_send << endl;
			ssize_t ret_val = send_msg(msg_to_send);
			if (ret_val == -1) {
				throw runtime_error(string("send: ") + strerror(errno));
			}
		}
	}
	return (1);
}

void Banbot::check_all_chan() {
	while (server_up) {
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
			if (search_word(buff)) {}
			else {
				cout << chatgpt(buff) << endl;
			}
			msg.erase(0, pos + 1);
		}
	}
}

void Banbot::routine() {
    cout << GREEN << "starting the bot routine" << RESET << endl;
	signal(SIGINT, handler_sigint);
	initial_chan_join();
    string msg;
    while(server_up) {
		check_all_chan();
	}
	msg = "QUIT :STOPING THE SERVER\r\n";
	send_msg(msg);
	clear_resize(msg);
	recv_msg(msg);
}