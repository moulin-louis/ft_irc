/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:45:46 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/27 16:04:37 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "Server.hpp"

void	Server::is_valid_chan_name(vector<string> params)
{
	if (params.size() != 1)	
		throw invalid_argument("join: invalid number of parameters");
	if (params[0][0] != '#')
		throw invalid_argument("join: channel name must start with #");
	if (params[0].size() > 50)
		throw invalid_argument("join: channel name too long");
}

void	Server::join(vector<string> params, Client& client)
{
	try {
		is_valid_chan_name(params);
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