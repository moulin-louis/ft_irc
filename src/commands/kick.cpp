/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 16:24:40 by mpignet           #+#    #+#             */
/*   Updated: 2023/04/17 18:10:59 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include <vector>

void	split_kick_version( vector<string>& list, string& str, const string& delimiter)
{
	size_t	pos;
	while ((pos = str.find(delimiter)) != string::npos) {
		list.push_back(str.substr(0, pos));
		str.erase(0, pos + delimiter.length());
	}
	list.push_back(str);
}

void	Server::process_kick_cmd(Channel& chan, const string& nick_user, Client& client, const string& reason)
{
	if (!chan.user_in_chan(client)) {
		this->add_rply_from_server(" :" + chan.getName() + " :You're not on that channel", client, "KICK", ERR_NOTONCHANNEL);
		return ;
	}
	Client& target = find_user(nick_user, client, "KICK");
	if (chan.user_in_chan(target))
	{
		this->notify_chan(chan, reason, "KICK", client);
		chan.removeClient(target);
		if (!target.channelsMember.empty())
		{
			for (size_t i = 0; i < target.channelsMember.size(); i++)
			{
				if (target.channelsMember[i] == chan.getName())
				{
					target.channelsMember.erase(find(target.channelsMember.begin(), target.channelsMember.end(), chan.getName()));
				}
			}
		}
		return;
	}
	this->add_rply_from_server(" :"  + target.getNickname() + " " + chan.getName() + " :They aren't on that channel", client, "KICK", ERR_USERNOTINCHANNEL);
}

void	Server::kick( const vector<string>& params, Client& client)
{
	try {
		if (params.size() < 2) {
			this->add_rply_from_server(":Not enough parameters", client, "KICK", ERR_NEEDMOREPARAMS);
			throw invalid_argument("kick: Not enough parameters");
		}
		if (!client.isOperator) {
			add_rply_from_server(":Permission Denied- You're not an IRC operator", client, "KICK", ERR_NOPRIVILEGES);
			throw invalid_argument("kick: Permission Denied- You're not an IRC operator");
		}
		vector <string> chan_list;
		vector<string> temp_vec(params);
		split_kick_version(chan_list, temp_vec[0], string(","));
		bool found = false;
		for (chan_iter it = this->chan_vec.begin(); it != this->chan_vec.end(); ++it) {
			if (it->getName() == chan_list[0]) {
				found = true;
				if (temp_vec.size() == 3)
					process_kick_cmd(*it, temp_vec[1], client, temp_vec[1]);
				else
					process_kick_cmd(*it, temp_vec[1], client, temp_vec[2]);
			}
		}
		if (!found)
			this->add_rply_from_server(" :" + chan_list[0] + " :No such channel", client, "KICK", ERR_NOSUCHCHANNEL);
	}
	catch ( exception& x ) {
		cout << RED << x.what() << RESET << endl;
	}
}