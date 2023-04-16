/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   chan_management.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 13:37:37 by mpignet           #+#    #+#             */
/*   Updated: 2023/04/16 16:27:21 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	add_cmd_client(const string& content, Client& client, const Client& author, const string&  cmd, const Channel& chan) {
	string msg = ":" + author.getNickname() + "!" + author.getUsername() + "@" + author.getHostname();
	msg += " " + cmd + " " + chan.getName() + " :" + content + endmsg;
	client.setBuff(client.getBuff() + msg);
}

void	Server::notify_chan(Channel& chan, const string& content, const string& cmd, const Client &author) {
	try {
		for ( cl_iter it = chan.clients.begin(); it != chan.clients.end(); ++it ) {
			if (*it == author.getFd()) {
				if (cmd == "JOIN" || cmd == "PART" || cmd == "QUIT" || cmd == "KICK" || cmd == "TOPIC") {
					add_cmd_client(content, (this->fd_map[*it]), author, cmd, chan);
				}
			}
			else
			{
				add_cmd_client(content, (this->fd_map[*it]), author, cmd, chan);
			}
		}
	}
	catch (exception& e) {
		cout << RED << e.what() << RESET << endl;
		return ;
	}
}
