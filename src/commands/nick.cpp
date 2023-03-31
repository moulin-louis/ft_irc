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
			add_rply_from_server(" :Nickname " + nickname + " is already in use", client, nickname, ERR_NICKNAMEINUSE);
			throw invalid_argument("nick: nickname already taken");
		}
	}
}
/*
static bool nickIsAlreadyInUse(const string &nickname, Server &server)
{
	for (client_iter it = server.fd_map.begin(); it != server.fd_map.end(); it++)
	{
		if (it->second.getNickname() == nickname)
		{
			cout << BLUE << "nickIsAlreadyInUse: " << nickname << " is already in use" << RESET << endl;
			return true;
		}
	}
	return false;
}*/

void	Server::nick(vector<string>& params, Client& client)
{
	//int i = 0;

	try {
		if (!client.passwd_provided) {
			add_rply_from_server(":You did not provide the password", client, "NICK", ERR_PASSWDMISMATCH);
			throw invalid_argument("nick: You did not provide the password");
		}
		if (params.empty() || params[0].empty()) {
			add_rply_from_server(":No nickname given", client, "NICK", ERR_NONICKNAMEGIVEN);
			throw invalid_argument("nick: No nickname given");
		}
		is_valid_nickname(params[0], client);
		string old_nickname = client.getNickname();

		std::string temp = params[0];/*
		while (nickIsAlreadyInUse(temp, *this))
		{
			temp += "_";
			i++;
		}
		if (i < 0)
		{
			add_rply_from_server(" :Nickname " + temp + " is already in use", client, temp, ERR_NICKNAMEINUSE);
			throw invalid_argument("nick: nickname already taken");
		}*/
		client.setNickname(temp);
		if (client.isRegistered) {
			string msg = string(":") + old_nickname + " !" + old_nickname + "@127.0.0.1 NICK :";
			msg += client.getNickname() + endmsg;
			client.setBuff(client.getBuff() + msg);
		}
	}
	catch(exception& e) {
		cout << RED << e.what() << RESET << endl;
		throw invalid_argument("nick: invalid nickname");
		return ;
	}
}