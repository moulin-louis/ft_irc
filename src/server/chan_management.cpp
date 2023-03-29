#include "Server.hpp"

void	Server::notify_chan(const string& content, const string& cmd, Client &client) {
	for ( cl_iter it = this->clients.begin(); it != this->clients.end(); it++ ) {
		this->add_cmd_client(content, (this->fd_map[*it]), client, cmd);
	}
}

void	Server::add_rply_from_server(string msg, Client& dest, string cmd, int code) {
	string dest_nick = dest.getNickname();
	if (dest_nick.empty())
		dest_nick = "*";
	dest_nick = " " + dest_nick;
	string result = ":localhost " + int_to_string(code) + dest_nick + " " + cmd + msg + endmsg;
	dest.setBuff(dest.getBuff() + result);
	return ;
}