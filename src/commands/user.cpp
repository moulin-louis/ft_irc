/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loumouli <loumouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 15:08:23 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/29 21:01:52 by loumouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::is_valid_username(string &username, Client& client) {
	if (username.size() > 9) {
		add_rply_from_server(":Username has invalid characters", client, " * ", ERR_NICKNAMEINUSE);
		throw invalid_argument("user: invalid username");
	}
	// On peut avoit plusieurs fois le même username
/*	for ( client_iter it = this->fd_map.begin(); it != this->fd_map.end(); it++) {
		if (it->second.getUsername() == username) {
			add_rply_from_server(":Username is already in use", client, " * ", ERR_NICKNAMEINUSE);
			throw invalid_argument("user: username already taken");
		}
	}*/
}

void	Server::user(vector<string>& params, Client& client) {
	try {
		if ( !client.passwd_provided ) {
			throw invalid_argument("user: no password given");
		}
		if ( client.isRegistered ) {
			add_rply_from_server(":Unauthorized command (already registered)", client, "USER", ERR_ALREADYREGISTRED);
			throw invalid_argument("user: Unauthorized command");
		}
		if (params.empty()) {
			add_rply_from_server(":Not enough parameters", client, "USER", ERR_NEEDMOREPARAMS);
			throw invalid_argument("user: Not enough parameters");
		}
		is_valid_username(params[0], client);
		client.setUsername(params[0]);
		params[3].erase(0, 1);
		client.setRealname(params[3]);
		client.isRegistered = true;
		add_rply_from_server(":Welcome to the Internet Relay Network " + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname(), client, "USER", RPL_WELCOME);
		add_rply_from_server(":Your host is " + this->_server_name + ", running version " + this->_server_version, client, "", RPL_YOURHOST);
		add_rply_from_server(":This server was created " + this->_server_up_date, client, "", RPL_CREATED);
		add_rply_from_server(":" + this->_server_name + " " + this->_server_version + " aoiw", client, "", RPL_MYINFO);
		vector<string> msg;
		motd(msg, client);
	}
	catch(exception& e) {
		cout << RED << e.what() << RESET << endl;
	}
}