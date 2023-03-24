/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   join.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armendi <armendi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:45:46 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/23 14:54:56 by armendi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "Server.hpp"

void	Server::is_valid_chan_name(vector<string> params)
{
	if (params.size() != 1)
	{	
		throw invalid_argument("join: invalid number of parameters");
	}
	if (params[0][0] != '#')
	{
		throw invalid_argument("join: channel name must start with #");
	}
	if (params[0].size() > 50)
	{
		throw invalid_argument("join: channel name too long");
	}
}

void	Server::join(vector<string> params, Client& client)
{
	try
	{
		is_valid_chan_name(params);
	}
	catch(exception& e)
	{
		cout << RED << e.what() << RESET << endl;
		return ;
	}
	for (map<string, Channel>::iterator it = this->chan_map.begin(); it != this->chan_map.end(); it++)
	{
		if (it->first == params[0])
		{
			string msg = "joined " + params[0];
			client.setBuff(client.getBuff() + "\n" + msg);
			it->second.addClient(client);
			return ;
		}
	}
	Channel new_channel(params[0], client);
	return ;
}