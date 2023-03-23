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
	string msg;
	msg = string(":localhost") + string("001") + client.getNickname() + string(" :Welcome to the Internet Relay Network ") + client.getNickname() + string("!") + client.getUsername() + string("@") + hostname;
	return (msg);
}

string	msg_invalid_nick(Client& client)
{	
	string msg;
	msg = string(":localhost") + string("433") + '*' + client.getNickname() + string(" :Nickname is already in use");
	return (msg);
}

bool is_valid_nickname(string &nickname)
{
	if (nickname.size() > 9)
		return (false);
	//check if nickname is already taken by another client
	return (true);
}

void	Server::nick(vector<string> params, Client& client)
{
	try
	{		
		if (!is_valid_nickname(params[0]))
			throw invalid_argument("invalid nickname");
	}
	catch(exception& e)
	{
		//send_client(msg_invalid_nick(client), client);
		return ;
	}
	client.setNickname(params[0]);
	//send_client(msg_welcome(client), client);
	return ;
}