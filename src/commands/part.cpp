/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loumouli <loumouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 15:16:43 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/29 21:03:41 by loumouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::process_part_cmd(Channel& chan, Client& client, string& reason)
{
	if (chan.user_in_chan(client) == true)
	{
		chan.removeClient(client);
		if (!reason.empty())
			this->notify_chan(chan.getName(), reason, "PART", client);
		else
			this->notify_chan(chan.getName(), "has left", "PART", client);
		return;
	}
	this->add_rply_from_server(" :" + chan.getName() + " :You're not on that channel", client, "PART", ERR_NOTONCHANNEL);
}

void	Server::part(vector<string> params, Client& client)
{
	try {
		if (params.size() < 1)
		{
			this->add_rply_from_server(":Not enough parameters", client, "PART", ERR_NEEDMOREPARAMS);
			throw  invalid_argument("part: Not enough parameters");
		}
		vector<string>	chans_to_part;
		size_t			pos;
		string delimiter = ",";
		while ((pos = params[0].find(delimiter)) != string::npos) {
			chans_to_part.push_back(params[0].substr(0, pos));
			params[0].erase(0, pos + delimiter.length());
		}
		chans_to_part.push_back(params[0]);
		for (vector<string>::iterator it = chans_to_part.begin(); it != chans_to_part.end(); it++) {
			for (chan_iter it2 = this->chan_vec.begin(); it2 != this->chan_vec.end(); it2++) {
				if (it2->getName() == *it) {
					process_part_cmd(*it2, client, params[1]);
					break ;
				}
				this->add_rply_from_server(" :" + *it + " :No such channel", client, "PART", ERR_NOSUCHCHANNEL);
		}
	}
	}
	catch ( exception& x ) {
		cout << RED << x.what() << RESET << endl;
	}
}