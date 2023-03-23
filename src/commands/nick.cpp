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

string	msg_welcome(Client& client)
{	
	string msg = ":localhost 001" + client.getNickname() + " :Welcome to the Internet Relay Network " + client.getNickname() + "!" + client.getUsername() + "@" + client.getHostname();
	return (msg);
}

string	msg_invalid_nick(Client& client)
{	
	string msg = ":localhost 433 * " + client.getNickname() + " :Nickname is already in use";
	return (msg);
}

void Server::is_valid_nickname(string &nickname)
{
	if (nickname.size() > 9)
		throw invalid_argument("invalid nickname");
	for (map<int, Client>::iterator it = this->fd_map.begin(); it != this->fd_map.end(); it++)
	{
		if (it->second.getNickname() == nickname)
			throw invalid_argument("nickname already taken");
	}
}

void	Server::nick(vector<string> params, Client& client)
{
	try
	{		
		is_valid_nickname(params[0]);
	}
	catch(exception& e)
	{
		string msg = msg_invalid_nick(client);
		send_client(msg, client);
		return ;
	}
	client.setNickname(params[0]);
	string msg = msg_welcome(client);
	send_client(msg, client);
	return ;
}