//
// Created by loumouli on 4/1/23.
//

#include "Banbot.hpp"


void Banbot::initial_connection() {
	cout << GREEN << "starting initial connection to server" << RESET << endl;
	this->sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->sfd == -1) {
		throw runtime_error(string("socket:") + strerror(errno));
	}
	cout << GREEN << "socket created" << RESET << endl;
	if (connect(this->sfd, (sockaddr*)&(this->sin), sizeof(sin)) == -1) {
		throw runtime_error(string("connect:") + strerror(errno));
	}
	cout << GREEN << "connection done" << RESET << endl;
	string msg = "PASS " + this->serv_pass + endmsg + "NICK " + this->bot_name + endmsg + "USER ";
	msg += "bot name user name";
	msg += endmsg;
	ssize_t ret_val = send_msg(msg);
	if (ret_val == -1) {
		throw runtime_error(string("send:") + strerror(errno));
	}
	clear_resize(msg);
	ret_val = recv_msg(msg);
	if (ret_val == -1) {
		throw runtime_error(string("recv:") + strerror(errno));
	}
	//NEED TO BECOME OPERATOR
	//NEED TO BECOME OPERATOR
	//NEED TO BECOME OPERATOR
	//NEED TO BECOME OPERATOR
	//NEED TO BECOME OPERATOR
	cout << GREEN << "Initial connection done" << RESET << endl;
}