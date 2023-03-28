/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server_tools.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loumouli <loumouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 19:52:41 by loumouli          #+#    #+#             */
/*   Updated: 2023/03/28 19:59:13 by loumouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

int epoll_ctl_add(int epfd, int fd, uint32_t events) {
	struct epoll_event ev = {};
	bzero(&ev.data, sizeof(ev.data));
	ev.events = events;
	ev.data.fd = fd;
	return (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev));
}

int	sendMessage(Client &client, const std::string& message) {
	return (send(client.getFd(), message.c_str(), message.length(), 0));
}

Client&	Server::find_user(string nick, Client client) {
	for (map<int, Client>::iterator it = this->fd_map.begin(); it != this->fd_map.end(); it++) {
		if (it->second.getNickname() == nick)
			return it->second;
	}
	add_rply_from_server(nick + " :No such nick/channel", client, "NICK", ERR_NOSUCHNICK);
	throw runtime_error("User not found");
}

Channel&	Server::find_channel(string name, Client client) {
	for (vector<Channel>::iterator it = this->chan_map.begin(); it != this->chan_map.end(); it++) {
		if (it->getName() == name)
			return *it;
	}
	add_rply_from_server(name + " :No such nick/channel", client, "JOIN", ERR_NOSUCHCHANNEL);
	throw runtime_error("User not found");
}

void	Server::add_cmd_client(string& content, Client& client, Client& author, string cmd) {
	string msg = ":" + author.getNickname() + "!" + author.getUsername() + "@" + author.getHostname() + " " + cmd + " :" + content + endmsg;
	client.setBuff(client.getBuff() + msg);
	return ;
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