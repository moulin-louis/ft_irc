/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   wallops.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnoulens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 18:48:28 by tnoulens          #+#    #+#             */
/*   Updated: 2023/04/13 19:22:26 by tnoulens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::wallops( const vector<string>& params, Client &client)
{
	string	msg;
	if (params.empty())
	{
		this->add_rply_from_server(" :No text to send", client , "WALLOPS", ERR_NEEDMOREPARAMS);
		return ;
	}
	if (!client.isOperator)
	{
		this->add_rply_from_server(" :Permission Denied - You're not an IRC operator", client , "WALLOPS", ERR_NOPRIVILEGES);
		return ;
	}
	else
	{
		msg = params[0];
		for (size_t i = 1; i < params.size(); i++)
		{
			msg += " ";
			msg += params[i];
		}
		msg.erase(string::size_type(0), 1);
	}
	string msg1 = ":" + client.getNickname() + \
					(client.getUsername().empty() ? "" : "!" + client.getUsername()) + \
					(client.getHostname().empty() ? "" : "@" + client.getHostname()) + \
					" WALLOPS :" + msg + endmsg;
	for (map<Socket, Client>::iterator it = this->fd_map.begin(); it != this->fd_map.end(); ++it)
	{
		if (!it->second.isLeaving && it->second.isRegistered && (it->second.getMode() & w))
			it->second.setBuff(it->second.getBuff() + msg1);
	}
}
