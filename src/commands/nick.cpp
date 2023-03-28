/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 15:08:23 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/27 17:06:46 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::is_valid_nickname(string &nickname, Client& client) {
	if (nickname.size() > 9) {
		add_rply_from_server(":Nickname is too long", client, "NICK", ERR_ERRONEUSNICKNAME);
		throw invalid_argument("nick: nickname too long");
	}
	for (unsigned int i = 0; i < nickname.size(); i++) {
		if (nickname[i] == ' ' || nickname[i] == '@') {
			add_rply_from_server(" :Erroneous nickname", client, "NICK", ERR_ERRONEUSNICKNAME);
			throw invalid_argument("nick: invalid character in nickname");
		}
	}
	for ( client_iter it = this->fd_map.begin(); it != this->fd_map.end(); it++) {
		if (it->second.getNickname() == nickname) {
			add_rply_from_server(" :Nickname is already in use", client, "NICK", ERR_NICKNAMEINUSE);
			throw invalid_argument("nick: nickname already taken");
		}
	}
}

void	Server::nick(vector<string> params, Client& client)
{
	if (!client.passwd_provided) {
		add_rply_from_server(":You did not provide the password", client, "NICK", ERR_PASSWDMISMATCH);
		return ;
	}
	if (params.empty() || params[0].empty()) {
		add_rply_from_server(":No nickname given", client, "NICK", ERR_NONICKNAMEGIVEN);
		return ;
	}
	try {
		is_valid_nickname(params[0], client);
	}
	catch(exception& e) {
		cout << RED << e.what() << RESET << endl;
		return ;
	}
	client.setNickname(params[0]);
	return ;
}