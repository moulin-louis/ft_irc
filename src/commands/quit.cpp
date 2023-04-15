/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnoulens <tnoulens@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:55:30 by loumouli          #+#    #+#             */
/*   Updated: 2023/03/29 15:50:05 by tnoulens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::quit( const vector<string>& params, Client &client)
{
	string msg;
	if (!params.empty())
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
                    " QUIT :closed connection " + msg + endmsg;
	client.setBuff(client.getBuff() + msg1);
	for (vector<string>::iterator it = client.channelsMember.begin(); it != client.channelsMember.end(); ++it)
	{
		Channel dest = find_channel(*it, client);
		this->notify_chan(dest, msg, "QUIT", client);
	}
	client.isLeaving = true;
	sendMessage(client, client.getBuff());
}
