/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armendi <armendi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:45:46 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/30 18:20:13 by armendi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"
#include "Server.hpp"
#include <vector>

void	Server::is_valid_chan_name(vector<string> params, Client& client)
{
	if (params.size() != 1) {
		add_rply_from_server(":Not enough parameters", client, "JOIN", ERR_NEEDMOREPARAMS);
		throw invalid_argument("join: invalid number of parameters");
	}
	if (params[0].size() > 50) {
		add_rply_from_server(":Channel name is too long", client, "JOIN", ERR_NAMETOOLONG);
		throw invalid_argument("join: channel name too long");
	}
}

void	Server::join(vector<string>& params, Client& client)
{
	try {
		is_valid_chan_name(params, client);
		vector<string>	chan_names;
		little_split(chan_names, params[0], ",");
		for (str_iter it = chan_names.begin(); it != chan_names.end(); it++) {
			bool chan_exists = false;		
			for ( chan_iter it2 = this->chan_vec.begin(); it2 != this->chan_vec.end(); it2++) {
				if (it2->getName() == *it) {
					it2->addClient(client);
					client.channelsMember.push_back(*it);
					this->notify_chan(it2->getName(), *it, "JOIN", client);
					vector<string> temp;
					temp.push_back(it2->getName());
					cout << "calling names here" << endl;
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
			notify_chan(new_channel.getName(), *it, "JOIN", client);
			for (client_iter it2 = this->fd_map.begin(); it2 != this->fd_map.end(); it2++)
			{
				if (it2->second.getMode() & B)
					notify_chan(new_channel.getName(), *it, "JOIN", it2->second);
			}
			vector<string> temp;
			temp.push_back(new_channel.getName());
			cout << "calling names here" << endl;
			this->names(temp, client);
		}
	}
	catch(exception& e) {
		cout << RED << e.what() << RESET << endl;
	}
}
