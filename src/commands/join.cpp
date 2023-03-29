/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:45:46 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/29 14:06:26 by mpignet          ###   ########.fr       */
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
	for ( chan_iter it = this->chan_vec.begin(); it != this->chan_vec.end(); it++) {
		if (it->getName() == params[0]) {
			it->addClient(client);
			this->notify_chan(it->getName(), params[0], "JOIN", client);
			return ;
		}
	}
	Channel new_channel(params[0], client);
	this->chan_vec.push_back(new_channel);
	notify_chan(new_channel.getName(), params[0], "JOIN", client);
	return ;
}
