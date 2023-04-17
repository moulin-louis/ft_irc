/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 16:46:02 by armendi           #+#    #+#             */
/*   Updated: 2023/04/16 17:59:40 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

void    Server::process_topic_cmd( const vector <string>& params, Client& client, Channel& chan)
{
	string topic;
	for ( vector<string>::const_iterator it = (params.begin() + 1); it != params.end(); it ++) {
		topic += *it;
		topic += ' ';
	}
    chan.setTopic(topic.substr(1));
    this->notify_chan(chan, chan.getTopic(), "TOPIC", client);
}

void	Server::topic( const vector<string>& params, Client& client )
{
    try {
		if (params.empty()) {
			add_rply_from_server(":Not enough parameters", client, "TOPIC", ERR_NEEDMOREPARAMS);
			throw invalid_argument("topic: invalid number of parameters");
		}
		if (params.size() == 1) {
			Channel& temp_chan = find_channel(params[0], client);
			add_cmd_client(temp_chan.getTopic(), client, client, "TOPIC", temp_chan);
			return ;
		}
		if (!client.isOperator) {
			add_rply_from_server(":You're not an operator", client, "TOPIC", ERR_NOPRIVILEGES);
			throw invalid_argument("topic: not an operator");
		}
		if (params.size() == 2 ) {
			for (chan_iter it = chan_vec.begin(); it != chan_vec.end(); ++it) {
				if (it->getName() == params[0]) {
					if (it->user_in_chan(client))
					{
						this->process_topic_cmd(params, client, *it);
						return ;
					}
				}
			}
			add_rply_from_server(params[0] + " :No such channel", client, "TOPIC", ERR_NOSUCHCHANNEL);
			throw invalid_argument("topic: no such channel");
		}
    }
    catch (const exception& e) {
        cout << RED << e.what() << RESET << endl;
    }
}