//
// Created by loumouli on 4/1/23.
//

#include "Banbot.hpp"
#include <sys/socket.h>

void Banbot::routine() {
    cout << GREEN << "starting the bot routine" << RESET << endl;
//	while(true) {
        ssize_t ret_val;
        string msg = string("LIST ") + endmsg;
        ret_val = send_msg(msg);
        if (ret_val == -11) {
            throw runtime_error(string("send:") + strerror(errno));
        }
        clear_resize(msg);
        ret_val = recv_msg(msg);
		if (ret_val == -1) {
			throw runtime_error(string("recv:") + strerror(errno));
		}
		msg.resize(ret_val);
		cout << msg << endl;
//	}
	msg.clear();
	msg = "QUIT TEST\r\n";
	send_msg(msg);
    clear_resize(msg);
    ret_val = recv_msg(msg);
    if (ret_val == -1) {
        throw runtime_error(string("recv:") + strerror(errno));
    }
    cout << "Bot deconected" << endl;
}