/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_tools.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 19:52:41 by loumouli          #+#    #+#             */
/*   Updated: 2023/04/21 15:37:27 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

string	displayTimestamp( ) {
	struct tm *timestruct;
	time_t timet;
	char buf[24];

	time(&timet);
	timestruct = localtime(&timet);
	strftime(buf, 24, "%d-%m-%Y at %H:%M:%S", timestruct);
	return (buf);
}

int epoll_ctl_add(int epfd, int fd, uint32_t events) {
	struct epoll_event ev = {};
	memset(&ev.data, 0, sizeof(ev.data));
	ev.events = events;
	ev.data.fd = fd;
	return (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev));
}

ssize_t 	sendMessage(const Client &client, const string& message) {
	return (send(client.getFd(), message.c_str(), message.length(), MSG_NOSIGNAL));
}

Client&	Server::find_user(const string& nick, Client& client, const string& cmd) {
	for (map<int, Client>::iterator it = this->fd_map.begin(); it != this->fd_map.end(); ++it) {
		if (it->second.getNickname() == nick)
			return it->second;
	}
	if (cmd == "PRIVMSG")
		add_cmd_client(nick, client, client, cmd, ERR_NOSUCHNICK);
	else
		add_rply_from_server(" : No such nick/channel", client, cmd, ERR_NOSUCHNICK);
	throw runtime_error("User not found");
}

Channel&	Server::find_channel(const string& name, Client& client) {
	for (vector<Channel>::iterator it = this->chan_vec.begin(); it != this->chan_vec.end(); ++it) {
		if (it->getName() == name)
			return *it;
	}
	add_rply_from_server(name + " :No such nick/channel", client, "", ERR_NOSUCHCHANNEL);
	throw runtime_error("Channel not found");
}

void	add_cmd_client(const string& content, Client& client, const Client& author, const string&  cmd) {
	string msg = ":" + author.getNickname() + "!" + author.getUsername() + "@" + author.getHostname() + " " + cmd + " " + client.getNickname() + " :" + content + endmsg;
	client.setBuff(client.getBuff() + msg);
}

void	Server::add_rply_from_server(const string&  msg, Client& dest, const string&  cmd, int code) {
	string dest_nick = dest.getNickname();
	if (dest_nick.empty())
		dest_nick = "*";
	dest_nick = " " + dest_nick;
	string result = ":" + this->_server_name + " " + int_to_string(code) + dest_nick + " " + cmd + msg + endmsg;
	dest.setBuff(dest.getBuff() + result);
}

void	Server::add_rply_from_server(const string&  msg, Client &client, Channel& dest, const string&  cmd, int code) {
	string result = ":" + this->_server_name + " " + int_to_string(code) + " " + client.getNickname() + " " + dest.getName() + " " + cmd + msg + endmsg;
	client.setBuff(client.getBuff() + result);
}

void	add_cmd_client(const string& content, Client& client, const Client& author, const string&  cmd, int code) {
	string msg = ":" + author.getNickname() + "!" + author.getUsername() + "@" + author.getHostname();
	msg += " " + int_to_string(code) + " " + cmd + " :" + content + endmsg;
	client.setBuff(client.getBuff() + msg);
}

void	little_split( vector<string> &list, string &str, const string& delimiter) {
	size_t	pos;
	while ((pos = str.find(delimiter)) != string::npos) {
		string buff = str.substr(0, pos);
		list.push_back(buff);
		str.erase(0, pos + delimiter.length());
	}
	list.push_back(str);
}

vector<string>& Server::get_botList() {
	return (this->_botList);
}

const char *Server::NicknameInUse::what() const throw() {
	return ("Nickname already in use");
}
