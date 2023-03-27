/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:45:46 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/27 17:06:41 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::is_valid_chan_name(vector<string> params, Client& client)
{
	if (params.size() != 1)
	{
		add_rply_from_server(":Not enough parameters", client, "JOIN", ERR_NEEDMOREPARAMS);
		throw invalid_argument("join: invalid number of parameters");
	}
	if (params[0][0] != '#')
	{
		add_rply_from_server(":Channel name must begin with '#'", client, "JOIN", ERR_INVALIDCHANNAME);
		throw invalid_argument("join: channel name must start with #");
	}
	if (params[0].size() > 50)
	{
		add_rply_from_server(":Channel name is too long", client, "JOIN", ERR_NAMETOOLONG);
		throw invalid_argument("join: channel name too long");
	}
}

void	Server::join(vector<string> params, Client& client)
{
	try {
		is_valid_chan_name(params, client);
	}
	catch(exception& e) {
		cout << RED << e.what() << RESET << endl;
		return ;
	}
	for (vector<Channel>::iterator it = this->chan_map.begin(); it != this->chan_map.end(); it++) {
		if (it->getName() == params[0]) {
			it->addClient(client);
			it->add_cmd_channel(params[0], client, "JOIN");
			return ;
		}
	}
	Channel new_channel(params[0], client);
	new_channel.add_cmd_channel(params[0], client, "JOIN");
	this->chan_map.push_back(new_channel);
	return ;
}