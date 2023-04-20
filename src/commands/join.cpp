/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:45:46 by mpignet           #+#    #+#             */
/*   Updated: 2023/04/17 16:33:41 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <vector>

void	Server::is_valid_chan_name(const vector<string>& params, Client& client)
{
	if (params.empty()) {
		add_rply_from_server(":Not enough parameters", client, "JOIN", ERR_NEEDMOREPARAMS);
		throw invalid_argument("join: invalid number of parameters");
	}
	if (params[0].size() > 50) {
		add_rply_from_server(":Channel name is too long", client, "JOIN", ERR_NAMETOOLONG);
		throw invalid_argument("join: channel name too long");
	}
}

void	Server::join( const vector<string>& params, Client& client)
{
	try {
		is_valid_chan_name(params, client);
		vector<string>	chan_names;
		string temp_str = params[0];
		little_split(chan_names, temp_str, ",");
		for (str_iter it = chan_names.begin(); it != chan_names.end(); ++it) {
			bool chan_exists = false;
			for ( chan_iter it2 = this->chan_vec.begin(); it2 != this->chan_vec.end(); ++it2) {
				if (it2->getName() == *it) {
					it2->addClient(client);
					client.channelsMember.push_back(*it);
					this->notify_chan(*it2, *it, "JOIN", client);
					vector<string> temp;
					temp.push_back(it2->getName());
					add_rply_from_server(it2->getTopic(), client, *it2, "", RPL_TOPIC);
					this->names(temp, client);
					chan_exists = true;
					break ;
				}
			}
			if (chan_exists)
				continue ;
			Channel new_channel(*it, client);
			this->chan_vec.push_back(new_channel);
			client.channelsMember.push_back(*it);
			notify_chan(new_channel, *it, "JOIN", client);
			vector<string> temp;
			temp.push_back(new_channel.getName());
			this->names(temp, client);
			add_rply_from_server(new_channel.getTopic(), client, new_channel, "", RPL_TOPIC);
			for (client_iter it3 = this->fd_map.begin(); it3 != this->fd_map.end(); ++it3 )
			{
				if (it3->second.getMode() & B)
				{
					it3->second.channelsMember.push_back(*it);
					this->chan_vec.back().addClient(it3->second);
					this->notify_chan(this->chan_vec.back(), *it, "JOIN", it3->second);
				}
			}
		}

	}
	catch(exception& e) {
		cout << RED << e.what() << RESET << endl;
	}
}
