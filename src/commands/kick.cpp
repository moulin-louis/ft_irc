/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armendi <armendi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 16:24:40 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/30 18:21:09 by armendi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

// void	prep_map(map<string, string> &map, string &chan_list, string &user_list, string delimiter)
// {
// 	string	buff_chan;
// 	string	buff_user;
// 	size_t	pos1;
// 	size_t	pos2;
// 	while ((pos1 = chan_list.find(delimiter)) != string::npos) {
// 		pos2 = user_list.find(delimiter);
// 		buff_chan = chan_list.substr(0, pos1);
// 		buff_user = user_list.substr(0, pos2);
// 		map.insert(make_pair(buff_chan, buff_user));
// 		chan_list.erase(0, pos1 + delimiter.length());
// 		user_list.erase(0, pos2 + delimiter.length());
// 	}
// 	map.insert(make_pair(chan_list, user_list));
// 	if (map.chan_list.empty() && !map.user_list.empty())
// 		this->add_rply_from_server(":No recipient given", client, "KICK", ERR_NORECIPIENT);
// 		throw invalid_argument("prep_map: Empty string");
// }

void	Server::process_kick_cmd(Channel& chan, string& nick_user, Client& client, string& reason)
{
	if (chan.user_in_chan(client) == false) {
		this->add_rply_from_server(" :" + chan.getName() + " :You're not on that channel", client, "KICK", ERR_NOTONCHANNEL);
		return ;
	}
	Client& target = find_user(nick_user, client, "KICK");
	if (chan.user_in_chan(target) == true)
	{
		chan.removeClient(target);
		if (!reason.empty())
			this->notify_chan(chan.getName(), reason, "KICK", client);
		else
			this->notify_chan(chan.getName(), nick_user, "KICK", client);
		return;
	}
	this->add_rply_from_server(" :"  + target.getNickname() + chan.getName() + " :They aren't on that channel", client, "KICK", ERR_USERNOTINCHANNEL);
	return ;
}

void	Server::kick(vector<string> params, Client& client)
{
	try {
		if (params.size() < 2) {
			this->add_rply_from_server(":Not enough parameters", client, "KICK", ERR_NEEDMOREPARAMS);
			throw invalid_argument("kick: Not enough parameters");
			return ;
		}
		if (!client.isOperator) {
			add_rply_from_server(":Permission Denied- You're not an IRC operator", client, "KICK", ERR_NOPRIVILEGES);
			throw invalid_argument("kick: Permission Denied- You're not an IRC operator");
			return ;
		}
		// map <string, string>	chan_user;
		// prep_map(chan_user, params[0], params[1], ",");
		vector <string> chan_list;
		vector <string> user_list;
		little_split(chan_list, params[0], ",");
		little_split(user_list, params[1], ",");
		if (chan_list.size() == 1) {
			for (chan_iter it = this->chan_vec.begin(); it != this->chan_vec.end(); it++) {
				bool found = false;
				if (it->getName() == chan_list[0]) {
					found = true;
					for (str_iter it2 = user_list.begin(); it2 != user_list.end(); it2++)
						process_kick_cmd(*it, *it2, client, params[2]);
				}
				if (found == true)
					continue ;
				this->add_rply_from_server(" :" + chan_list[0] + " :No such channel", client, "KICK", ERR_NOSUCHCHANNEL);
			}
		}
		else {			
			if (chan_list.size() != user_list.size()) {
				this->add_rply_from_server(":No recipient given", client, "KICK", ERR_NORECIPIENT);
				throw invalid_argument("kick: No recipient given");
			}
			int	i = 0;
			for (str_iter it = chan_list.begin(); it != chan_list.end(); it++) {
				bool found = false;
				for (chan_iter it2 = this->chan_vec.begin(); it2 != this->chan_vec.end(); it2++) {
					if (it2->getName() == *it) {
						process_kick_cmd(*it2, user_list[i], client, params[2]);
						found = true;
						break ;
					}
				}
				if (found == false)
					this->add_rply_from_server(" :" + *it + " :No such channel", client, "KICK", ERR_NOSUCHCHANNEL);
				i++;
			}
		}
	}
	catch ( exception& x ) {
		cout << RED << x.what() << RESET << endl;
	}
}