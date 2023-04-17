/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnoulens <tnoulens@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:55:30 by loumouli          #+#    #+#             */
/*   Updated: 2023/03/29 15:50:05 by tnoulens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <stdexcept>

void Server::names( const vector<string> &params, Client &client) {
	try {
		if (params.empty()) {
			for ( chan_iter it = this->chan_vec.begin(); it != this->chan_vec.end(); it++) {

				for (vec_sock_iter it2 = it->clients.begin(); it2 != it->clients.end(); ++it2) {
					if (this->fd_map[*it2].getNickname() == client.getNickname()) {
						continue ;
					}
					if (!(this->fd_map[*it2].getMode() & i)) {
						string msg = ":";
						msg += this->_server_name + " ";
						msg += int_to_string(RPL_NAMREPLY) + " ";
						msg += client.getNickname() + " = ";
						msg += params[0] + " :";
						msg += client.getNickname() + " ";
						msg += this->fd_map[*it2].getNickname() + endmsg;
						client.setBuff(client.getBuff() + msg);
					}
				}

			}
			return ;
		}
		Channel chan = find_channel(params[0], client);
		for (vec_sock_iter it = chan.clients.begin(); it != chan.clients.end(); ++it) {
			if (this->fd_map[*it].getNickname() == client.getNickname()) {
				continue ;
			}
			if (!(this->fd_map[*it].getMode() & i)) {
				string msg = ":";
				msg += this->_server_name + " ";
				msg += int_to_string(RPL_NAMREPLY) + " ";
				msg += client.getNickname() + " = ";
				msg += params[0] + " :";
				msg += client.getNickname() + " ";
				msg += this->fd_map[*it].getNickname() + endmsg;
				client.setBuff(client.getBuff() + msg);
			}
		}
	}
	catch (exception &x) {
		cout << RED << x.what() << RESET << endl;
	}
	string msg = ":";
	msg += this->_server_name + " ";
	msg += int_to_string(RPL_ENDOFNAMES) + " ";
	msg += client.getNickname() + " ";
	msg += params[0] + " :";
	msg += "End of /NAMES list." + endmsg;
	client.setBuff(client.getBuff() + msg);
}