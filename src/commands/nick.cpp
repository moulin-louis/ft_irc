/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loumouli <loumouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 15:08:23 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/22 17:50:13 by loumouli         ###   ########.fr       */
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
		throw invalid_argument("invalid nickname");
	}
	for (map<int, Client>::iterator it = this->fd_map.begin(); it != this->fd_map.end(); it++)
	{
		if (it->second.getNickname() == nickname)
		{
			string msg = ":localhost 433 * " + nickname + " :Nickname is already in use";
			send_client(msg, client);
			throw invalid_argument("nickname already taken");
		}
	}
}

void	Server::nick(vector<string> params, Client& client)
{
	try
	{		
		is_valid_nickname(params[0], client);
	}
	catch(exception& e)
	{
		return ;
	}
	client.setNickname(params[0]);
	// string msg = msg_welcome(client);
	// send_client(msg, client);
	return ;
}