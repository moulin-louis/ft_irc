/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armendi <armendi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 16:24:40 by mpignet           #+#    #+#             */
/*   Updated: 2023/04/02 12:44:21 by armendi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	split_kick_version(vector<string> &list, string &str, const string& delimiter)
{
	string	buff;
	size_t	pos;
	while ((pos = str.find(delimiter)) != string::npos) {
		buff = "#" + str.substr(0, pos);
		list.push_back(buff);
		str.erase(1, pos + delimiter.length());
	}
	list.push_back("#" + str);
}

void	Server::process_kick_cmd(Channel& chan, string& nick_user, Client& client, string& reason)
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
		return;
	}
	this->add_rply_from_server(" :"  + target.getNickname() + chan.getName() + " :They aren't on that channel", client, "KICK", ERR_USERNOTINCHANNEL);
}

void	Server::kick(vector<string>& params, Client& client)
{
	cout << "params[0]" << params[0] << endl;
	cout << "params[1]" << params[1] << endl;
	cout << "params[2]" << params[2] << endl;
	cout << "params[3]" << params[3] << endl;
	cout << params.size() << endl;
	try {
		if (params.size() < 3) {
			this->add_rply_from_server(":Not enough parameters", client, "KICK", ERR_NEEDMOREPARAMS);
			throw invalid_argument("kick: Not enough parameters");
		}
		if (!client.isOperator) {
			add_rply_from_server(":Permission Denied- You're not an IRC operator", client, "KICK", ERR_NOPRIVILEGES);
			throw invalid_argument("kick: Permission Denied- You're not an IRC operator");
		}
		vector <string> chan_list;
		vector <string> user_list;
		split_kick_version(chan_list, params[1], ",");
		params[2].erase(0, 1);
		little_split(user_list, params[2], ",");
		if (chan_list.size() == 1) {
			bool found = false;
			for (chan_iter it = this->chan_vec.begin(); it != this->chan_vec.end(); it++) {
				if (it->getName() == chan_list[0]) {
					found = true;
					for (str_iter it2 = user_list.begin(); it2 != user_list.end(); it2++)
					{
						if (params.size() == 3)
							process_kick_cmd(*it, *it2, client, *it2);
						else
							process_kick_cmd(*it, *it2, client, params[3]);
					}
					break ;
				}
			}
			if (found == false)
				this->add_rply_from_server(" :" + chan_list[0] + " :No such channel", client, "KICK", ERR_NOSUCHCHANNEL);
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
						process_kick_cmd(*it2, user_list[i], client, params[3]);
						found = true;
						break ;
					}
				}
				if (found )
					this->add_rply_from_server(" :" + *it + " :No such channel", client, "KICK", ERR_NOSUCHCHANNEL);
				i++;
			}
		}
	}
	catch ( exception& x ) {
		cout << RED << x.what() << RESET << endl;
	}
}