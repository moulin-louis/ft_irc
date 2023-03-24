/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 15:08:23 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/24 16:38:20 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "Server.hpp"

string	msg_welcome(Client& client)
{
	string msg = ":localhost 001 " + client.getNickname() + " :Welcome to the Internet Relay Network " + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname() + endmsg;
	return (msg);
}

void Server::is_valid_username(string &username, Client& client)
{
	if (username.size() > 9) {
		string msg = ":localhost 433 * " + username + " :Username has invalid characters" + endmsg;
		client.setBuff(client.getBuff() + msg);
		throw invalid_argument("user: invalid username");
	}
	for (map<int, Client>::iterator it = this->fd_map.begin(); it != this->fd_map.end(); it++) {
		if (it->second.getUsername() == username) {
			string msg = ":localhost 433 * " + username + " :Username is already in use" + endmsg;
			client.setBuff(client.getBuff() + msg);
			throw invalid_argument("user: username already taken");
		}
	}
}

void	Server::user(vector<string> params, Client& client)
{
	if (client.passwd_provided == false) {
		return ;
	}
	try {
		is_valid_username(params[0], client);
	}
	catch(exception& e) {
		cout << RED << e.what() << RESET << endl;
		return ;
	}
	client.setUsername(params[0]);
	client.isRegistered = true;
	client.setBuff(client.getBuff() + msg_welcome(client));
	return ;
}