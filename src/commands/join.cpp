/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:45:46 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/24 16:15:13 by mpignet          ###   ########.fr       */
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

void	Channel::notify_clients(Channel& channel, Client& client, string cmd)
{
	for (vector<Client>::iterator it = channel.clients.begin(); it != channel.clients.end(); it++) {
		string msg = ":" + client.getNickname() + " " + cmd + " " + channel.getName() + endmsg;
		it->setBuff(it->getBuff() + msg);
	}
}

void	Server::join(vector<string> params, Client& client)
{
	if ( !client.isRegistered ) {
		cout << RED << "join: not registered" << RESET << endl;
		string msg = ":localhost " + int_to_string(ERR_NOTREGISTERED) + " * JOIN:You have not registered.:" + endmsg;
		client.setBuff(client.getBuff() + msg);
		return ;
	}
	try {
		is_valid_chan_name(params);
	}
	catch(exception& e) {
		cout << RED << e.what() << RESET << endl;
		return ;
	}
	for (map<string, Channel>::iterator it = this->chan_map.begin(); it != this->chan_map.end(); it++) {
		if (it->first == params[0]) {
			it->second.addClient(client);
			it->second.notify_clients(it->second, client, "JOIN");
			return ;
		}
	}
	Channel new_channel(params[0], client);
	this->chan_map.insert(pair<string, Channel>(params[0], new_channel));
	return ;
}