/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armendi <armendi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 15:16:43 by mpignet           #+#    #+#             */
/*   Updated: 2023/04/02 12:07:02 by armendi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::process_part_cmd(Channel& chan, Client& client)
{
	if (chan.user_in_chan(client))
	{
		this->notify_chan(chan.getName(), client.getNickname(), "PART", client);
		chan.removeClient(client);
		return;
	}
	this->add_rply_from_server(" :" + chan.getName() + " :You're not on that channel", client, "PART", ERR_NOTONCHANNEL);
}

void	Server::process_part_cmd(Channel& chan, Client& client, string& reason)
{
	if (chan.user_in_chan(client))
	{
		this->notify_chan(chan.getName(), reason, "PART", client);
		chan.removeClient(client);
		for (vector<string>::iterator itr = client.channelsMember.begin(); itr != client.channelsMember.end(); itr++)
		{
			if (*itr == chan.getName())
			{
				client.channelsMember.erase(itr);
				break;
			}
		}
	}
	this->add_rply_from_server(" :" + chan.getName() + " :You're not on that channel", client, "PART", ERR_NOTONCHANNEL);
}

void	Server::part(vector<string>& params, Client& client)
{
	try {
//		if (params.size() == 1)
//		{
//			this->add_rply_from_server(":Not enough parameters", client, "PART", ERR_NEEDMOREPARAMS);
//			throw  invalid_argument("part: Not enough parameters");
//		}
		vector<string>	chans_to_part;
		size_t			pos;
		string delimiter = ",";
		params[1].erase(0, 1);
		while ((pos = params[1].find(delimiter)) != string::npos) {
			chans_to_part.push_back("#" + params[1].substr(0, pos));
			params[1].erase(1, pos + delimiter.length());
		}
		chans_to_part.push_back("#" + params[1]);
		for ( str_iter it = chans_to_part.begin(); it != chans_to_part.end(); it++) {
			bool chan_exists = false;
			for (chan_iter it2 = this->chan_vec.begin(); it2 != this->chan_vec.end(); it2++) {
				if (it2->getName() == *it) {
					if (params.size() > 2)
						process_part_cmd(*it2, client, params[2]);
					else
						process_part_cmd(*it2, client);
					chan_exists = true;
					break ;
				}
			}
			if (!chan_exists)
				this->add_rply_from_server(" :" + *it + " :No such channel", client, "PART", ERR_NOSUCHCHANNEL);
		}
	}
	catch ( exception& x ) {
		cout << RED << x.what() << RESET << endl;
	}
}