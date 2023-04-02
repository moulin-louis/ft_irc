/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chan_management.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armendi <armendi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 13:37:37 by mpignet           #+#    #+#             */
/*   Updated: 2023/04/02 12:06:27 by armendi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::add_cmd_client(const string& content, Client& client, Client& author, const string&  cmd, Channel& chan) {
	string msg = ":" + author.getNickname() + "!" + author.getUsername() + "@" + author.getHostname();
	msg += " " + cmd + " " + chan.getName() + " :" + content + endmsg;
	client.setBuff(client.getBuff() + msg);
}

void	Server::notify_chan(const string& chan, const string& content, const string& cmd, Client &author) {
	try {
		Channel& channel = this->find_channel(chan, author);
		for ( cl_iter it = channel.clients.begin(); it != channel.clients.end(); it++ ) {
			if (*it == author.getFd()) {
				if (cmd == "JOIN" || cmd == "PART" || cmd == "QUIT" || cmd == "KICK")
					this->add_cmd_client(content, (this->fd_map[*it]), author, cmd, channel);
			}
			else
				this->add_cmd_client(content, (this->fd_map[*it]), author, cmd, channel);
		}
	}
	catch (exception& e) {
		cout << RED << e.what() << RESET << endl;
		return ;
	}
}
