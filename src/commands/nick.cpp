/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 15:08:23 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/24 16:34:07 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "Server.hpp"

void Server::is_valid_nickname(string &nickname, Client& client)
{
	if (nickname.size() > 9) {
		string msg = ":localhost" + int_to_string(ERR_ERRONEUSNICKNAME) + " * " + nickname + " :Nickname has invalid characters" + endmsg;
		client.setBuff(client.getBuff() + msg);
		throw invalid_argument("nick: nickname too long");
	}
	for (unsigned int i = 0; i < nickname.size(); i++) {
		if (i == ' ' || i == '@') {
			string msg = ":localhost" + int_to_string(ERR_ERRONEUSNICKNAME) + " * " + nickname + " :Erroneous nickname" + endmsg;
			client.setBuff(client.getBuff() + msg);
			throw invalid_argument("nick: invalid character in nickname");
		}
	}
	for (map<int, Client>::iterator it = this->fd_map.begin(); it != this->fd_map.end(); it++) {
		if (it->second.getNickname() == nickname) {
			string msg = ":localhost" + int_to_string(ERR_NICKNAMEINUSE) + " * " + nickname + " :Nickname is already in use" + endmsg;
			client.setBuff(client.getBuff() + msg);
			throw invalid_argument("nick: nickname already taken");
		}
	}
}

void	Server::nick(vector<string> params, Client& client)
{
	if (!client.passwd_provided) {
		string msg = ":localhost " + int_to_string(ERR_PASSWDMISMATCH) + " * :You did not provide the password" + endmsg;
		client.setBuff(client.getBuff() + msg);
		return ;
	}
	if (params.empty() || params[0].empty()) {
		string msg = ":localhost" + int_to_string(ERR_NONICKNAMEGIVEN) + " * " + " :No nickname given" + endmsg;
		client.setBuff(client.getBuff() + msg);
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