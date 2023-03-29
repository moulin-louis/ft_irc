/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   part.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 15:16:43 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/29 15:43:34 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

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
					it2->removeClient(client);
					if (!params[1].empty())
						this->notify_chan(it2->getName(), params[1], "PART", client);
					else
						this->notify_chan(it2->getName(), "has left", "PART", client);
					break ;
				}
			}
		}
	}
	catch ( exception& x ) {
		cout << RED << x.what() << RESET << endl;
	}
}