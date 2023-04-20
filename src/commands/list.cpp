/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   list.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: waxxy <waxxy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 15:49:09 by tnoulens          #+#    #+#             */
/*   Updated: 2023/03/30 13:49:19 by waxxy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

static void displayAllChannels(Server &server, Client &client, vector<Channel> &chan_vec)
{
	for (chan_iter it = chan_vec.begin(); it != chan_vec.end(); ++it)
	{
		if (!(it->getMode() & p))
			server.add_rply_from_server(":" + it->getName() + " " + itostr((int)it->clients.size()) + " " + it->getTopic(), client, "", RPL_LIST);
	}
	server.add_rply_from_server(":End of LIST", client, "", RPL_LISTEND);
}

static void	getSpecifiedChannels(Server &server, Client &client, vector<Channel> &chan_vec, const vector<string> &params)
{
	istringstream	ss(params[0]);
	string 		chan_name;
	vector<string>	chan_names;

	while ( getline(ss, chan_name, ','))
	{
		if (chan_name[0] != '#')
			continue;
		else
			for (chan_iter it = chan_vec.begin(); it != chan_vec.end(); ++it)
				if (it->getName() == chan_name && find(chan_names.begin(), chan_names.end(), chan_name) == chan_names.end() && (!(it->getMode() & p)))
				{
					chan_names.push_back(chan_name);
					server.add_rply_from_server(":" + it->getName() + " " + itostr(it->clients.size()) + " " + it->getTopic(), client, "", RPL_LIST);
				}
	}
	server.add_rply_from_server(":End of LIST", client, "", RPL_LISTEND);
}

void	Server::list( const vector<string>& params, Client &client)
{
	if (params.empty())
    {
        cerr << RED << "list: invalid number of parameters" << RESET << endl;
        return ;
    }
	if (!params[0].length())
		::displayAllChannels(*this, client, this->chan_vec);
	else if (params.size() == 1)
		::getSpecifiedChannels(*this, client, this->chan_vec, params);
	else
	{
		//Theoretically, we should check on the server netwok if the server exists,
		// but here, we only check if the specified server name is the same as _server_name
		if (params[1] != this->_server_name)
			add_rply_from_server(":No such server", client, "LIST", ERR_NOSUCHSERVER);
		else
			::getSpecifiedChannels(*this, client, this->chan_vec, params);
	}
}
