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

void Server::quit(vector<string>& params, Client &client)
{
        string msg1;
		if (!params.empty())
		{
			msg1 = params[0];
			for (size_t i = 1; i < params.size(); i++)
			{
				msg1 += " ";
				msg1 += params[i];
			}
			msg1.erase(string::size_type(0), 1);
		}
		string msg = ":" + client.getNickname() +\
					(client.getUsername().empty() ? "" : "!" + client.getUsername()) +\
					(client.getHostname().empty() ? "" : "@" + client.getHostname()) +\
					" QUIT :closed connection " + msg1 + endmsg;
		client.setBuff(client.getBuff() + msg);
		sendMessage(client, client.getBuff());
		this->_disconect_client(client.getFd());
		throw runtime_error("client lost connection: " + msg1);
}
