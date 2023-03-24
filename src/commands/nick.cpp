/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armendi <armendi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 15:08:23 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/23 14:45:19 by armendi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "Server.hpp"

void Server::is_valid_nickname(string &nickname, Client& client)
{
	if (nickname.size() > 9)
	{
		string msg = ":localhost 433 * " + nickname + " :Nickname has invalid characters";
		send_client(msg, client);
		throw invalid_argument("nick: invalid nickname");
	}
	for (map<int, Client>::iterator it = this->fd_map.begin(); it != this->fd_map.end(); it++)
	{
		if (it->second.getNickname() == nickname)
		{
			string msg = ":localhost 433 * " + nickname + " :Nickname is already in use";
			send_client(msg, client);
			throw invalid_argument("nick: nickname already taken");
		}
	}
}

void	Server::nick(vector<string> params, Client& client)
{
	if (params.size() == 0 || params[0].empty())
	{
		throw	invalid_argument("nick: wrong number of parameters");
		return ;
	}
	try
	{		
		is_valid_nickname(params[0], client);
	}
	catch(exception& e)
	{
//		send_client(msg, client);
		return ;
	}
	cout << "testing done" << endl;
	client.setNickname(params[0]);
	// string msg = msg_welcome(client);
	// send_client(msg, client);
	return ;
}