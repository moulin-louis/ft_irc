//
// Created by loumouli on 4/1/23.
//

#include "Banbot.hpp"

void Banbot::initial_connection() {
	cout << GREEN << "starting initial connection to server" << RESET << endl;
	this->sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->sfd == -1) {
		throw runtime_error(string("socket: ") + strerror(errno));
	}
	cout << GREEN << "socket created" << RESET << endl;
	if (connect(this->sfd, (sockaddr*)&(this->sin), sizeof(sin)) == -1) {
		throw runtime_error(string("connect: ") + strerror(errno));
	}
	cout << GREEN << "connection done" << RESET << endl;

	string msg = "PASS " + this->serv_pass + endmsg + "NICK " + this->bot_nickname + endmsg + "USER ";
	msg += this->bot_nickname + " " + this->bot_nickname + " localost :" + "BanBot";
	msg += endmsg;
	ssize_t ret_val = send_msg(msg);
	if (ret_val == -1) {
		throw runtime_error(string("send: ") + strerror(errno));
	}
	clear_resize(msg);
	ret_val = recv_msg(msg);
	if (ret_val == -1) {
		throw runtime_error(string("recv: ") + strerror(errno));
	}
	msg.clear();

	msg = "OPER " + this->bot_nickname + " " + this->admin_pasword + endmsg;
	ret_val = send_msg(msg);
	if (ret_val == -1) {
		throw runtime_error(string("send: ") + strerror(errno));
	}
	clear_resize(msg);
	ret_val = recv_msg(msg);
	if (ret_val == -1) {
		throw runtime_error(string("send: ") + strerror(errno));
	}
	msg.resize(ret_val);
	if (msg.find("381") == string::npos)
		throw runtime_error("Cant become an operator, check your config");
//	ajout du mode B au bot
	msg = "MODE " + this->bot_nickname + " " + "+B" + endmsg;
	ret_val = send_msg(msg);
	if (ret_val == -1) {
		throw runtime_error(string("send: ") + strerror(errno));
	}
	clear_resize(msg);
	ret_val = recv_msg(msg);
	if (ret_val == -1) {
		throw runtime_error(string("send: ") + strerror(errno));
	}
	msg.resize(ret_val);
	if (msg.find("481") != string::npos)
		throw runtime_error("Cant switch to B mode, check your config");
	cout << GREEN << "Initial connection done" << RESET << endl;
}