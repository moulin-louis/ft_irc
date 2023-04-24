//
// Created by tnoulens on 4/24/23.
//

#include "Server.hpp"

void    Server::away( const vector<string>& params, Client &client)
{
	string	msg;
	if (params.empty())
	{
		client.unSetMode(a);
		client.setAway("");
		this->add_rply_from_server(" :Now available", client , "", RPL_UNAWAY);
		return ;
	}
	else
	{
		client.setMode(a);
		msg = params[0];
		for (size_t i = 1; i < params.size(); i++)
		{
			msg += " ";
			msg += params[i];
		}
		client.setAway(msg);
		msg.erase(string::size_type(0), 1);
		add_rply_from_server(msg, client, "", RPL_NOWAWAY);
	}
}