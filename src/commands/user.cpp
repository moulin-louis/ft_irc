/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 15:08:23 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/29 17:12:55 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "Server.hpp"

void Server::is_valid_username(string &username, Client& client) {
	if (username.size() > 9) {
		string msg = ":localhost" + int_to_string(ERR_NICKNAMEINUSE) + " * "  + username + " :Username has invalid characters" + endmsg;
		client.setBuff(client.getBuff() + msg);
		throw invalid_argument("user: invalid username");
	}
	// for ( client_iter it = this->fd_map.begin(); it != this->fd_map.end(); it++) {
	// 	if (it->second.getUsername() == username) {
	// 		string msg = ":localhost " + int_to_string(ERR_NICKNAMEINUSE) + " * " + username + " :Username is already in use" + endmsg;
	// 		client.setBuff(client.getBuff() + msg);
	// 		throw invalid_argument("user: username already taken");
	// 	}
	// }
}

void	Server::user(vector<string> params, Client& client) {
	if ( !client.passwd_provided ) {
		return ;
	}
	if ( client.isRegistered ) {
		add_rply_from_server(":Unauthorized command (already registered)", client, "USER", ERR_ALREADYREGISTRED);
		return ;
	}
	if (params.empty()) {
		add_rply_from_server(":Not enough parameters", client, "USER", ERR_NEEDMOREPARAMS);
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
	add_rply_from_server(":Welcome to the Internet Relay Network " + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname(), client, "USER", RPL_WELCOME);
	return ;
}