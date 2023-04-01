//
// Created by loumouli on 4/1/23.
//

#include "Banbot.hpp"

void Banbot::routine() {
	cout << GREEN << "starting the bot routine" << RESET << endl;
//	while(true) {
		int ret_val;
		string msg = string("LIST ") + endmsg;
		ret_val = send_msg(msg);
		if (ret_val == -11) {
			throw runtime_error(string("send:") + strerror(errno));
		}
		msg.clear();
		msg.resize(512);
		ret_val = recv(this->sfd, (void *)msg.c_str(), 512, 0);
		if (ret_val == -1) {
			throw runtime_error(string("recv:") + strerror(errno));
		}
		msg.resize(ret_val);
		cout << msg << endl;
//	}
	msg.clear();
	msg = "QUIT TEST\r\n";
	send_msg(msg);

}