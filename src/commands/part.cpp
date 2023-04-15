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
		this->notify_chan(chan, client.getNickname(), "PART", client);
		chan.removeClient(client);
        if (!client.channelsMember.empty())
        {
            for (size_t i = 0; i < client.channelsMember.size(); i++)
			{
                if (client.channelsMember[i] == chan.getName())
				{
                    client.channelsMember.erase(find(client.channelsMember.begin(), client.channelsMember.end(), chan.getName()));
                }
            }
        }
		return;
	}
	this->add_rply_from_server(" :" + chan.getName() + " :You're not on that channel", client, "PART", ERR_NOTONCHANNEL);
}

void	Server::process_part_cmd(Channel& chan, Client& client, string& reason)
{
	if (chan.user_in_chan(client))
	{
		this->notify_chan(chan, reason, "PART", client);
		chan.removeClient(client);
		if (!client.channelsMember.empty())
		{
			for (size_t i = 0; i < client.channelsMember.size(); i++)
			{
				if (client.channelsMember[i] == chan.getName())
				{
					client.channelsMember.erase(find(client.channelsMember.begin(), client.channelsMember.end(), chan.getName()));
				}
			}
		}
		return ;
	}
	this->add_rply_from_server(" :" + chan.getName() + " :You're not on that channel", client, "PART", ERR_NOTONCHANNEL);
}

void	Server::part(vector<string>& params, Client& client)
{
	try
    {
		if (params.empty())
		{
			this->add_rply_from_server(":Not enough parameters", client, "PART", ERR_NEEDMOREPARAMS);
			throw  invalid_argument("part: Not enough parameters");
		}
		vector<string>	chans_to_part;
//		irssi always appends the channel of the active channel window as a parameter
//		if the active window is the main window, and no parameter is given then an error
//		is thrown by irssi, we push back it directly to the vector as it already has the #
		if (params.size() == 1)
			chans_to_part.push_back(params[0]);
//		if there are parameters given then we get these channels to part instead of the active one:
		else
		{
            size_t			pos;
            string delimiter = ",";
			params[1].erase(0, 1);
			while ((pos = params[1].find(delimiter)) != string::npos)
            {
                if (params[1][0] == '#')
    				chans_to_part.push_back(params[1].substr(0, pos));
                else
                    chans_to_part.push_back("#" + params[1].substr(0, pos));
				params[1].erase(0, pos + delimiter.length());
			}
			if (params[1][0] == '#')
				chans_to_part.push_back(params[1]);
			else
				chans_to_part.push_back("#" + params[1]);
		}
		for ( str_iter it = chans_to_part.begin(); it != chans_to_part.end(); it++)
        {
			bool chan_exists = false;
			for (chan_iter it2 = this->chan_vec.begin(); it2 != this->chan_vec.end(); it2++)
            {
				if (it2->getName() == *it)
                {
					if (params.size() > 2)
					{
						string reason = params[2];
						for (size_t i = 3; i < params.size(); i++)
							reason += " " + params[i];
						process_part_cmd(*it2, client, reason);
					}
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
	catch ( exception& x )
    {
		cout << RED << x.what() << RESET << endl;
	}
}
