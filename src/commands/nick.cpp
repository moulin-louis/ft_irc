/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 15:08:23 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/24 13:18:54 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "Server.hpp"

void Server::is_valid_nickname(string &nickname, Client& client)
{
	if (nickname.size() > 9)
	{
		string msg = ":localhost 433 * " + nickname + " :Nickname has invalid characters";
		client.setBuff(client.getBuff() + "\n" + msg);
		throw invalid_argument("nick: invalid nickname");
	}
	for (map<int, Client>::iterator it = this->fd_map.begin(); it != this->fd_map.end(); it++)
	{
		if (it->second.getNickname() == nickname)
		{
			string msg = ":localhost 433 * " + nickname + " :Nickname is already in use";
			client.setBuff(client.getBuff() + "\n" + msg);
			throw invalid_argument("nick: nickname already taken");
		}
	}
}

void	Server::nick(vector<string> params, Client& client)
{
	try {
		if (params.size() == 0 || params[0].empty())
			throw	invalid_argument("nick: wrong number of parameters");
		is_valid_nickname(params[0], client);
	}
	catch(exception& e) {
		cout << RED << e.what() << RESET << endl;
		return ;
	}
	client.setNickname(params[0]);
	return ;
}