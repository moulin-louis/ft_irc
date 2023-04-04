/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 16:46:02 by armendi           #+#    #+#             */
/*   Updated: 2023/03/29 14:01:47 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::process_topic_cmd(vector <string>& params, Client& client, Channel& chan)
{
    if (params.size() == 2) {
        chan.setTopic(params[1]);
        this->notify_chan(chan.getName(), params[1], "TOPIC", client);
	}
    else {
        if (chan.getTopic().empty())
            this->add_rply_from_server(chan.getName() + ":No topic is set", client, "", RPL_NOTOPIC);
        else
            this->add_rply_from_server(chan.getName() + " :" + chan.getTopic(), client, "", RPL_TOPIC);
    }
}

void	Server::topic( vector<string>& params, Client& client )
{
    try {
        if (params.size() != 1) {
            add_rply_from_server(":Not enough parameters", client, "TOPIC", ERR_NEEDMOREPARAMS);
            throw invalid_argument("join: invalid number of parameters");
        }
        if (params[0][0] != '#') {
            add_rply_from_server(params[0] + " :Invalid channel name", client, "TOPIC", ERR_NOSUCHCHANNEL);
            throw invalid_argument("join: invalid channel name");
        }
        for (chan_iter it = chan_vec.begin(); it != chan_vec.end(); it++) {
            if (it->getName() == params[0])
            {
                if (it->user_in_chan(client))
                {
                    this->process_topic_cmd(params, client, *it);
                    return ;
                }
                else
                {
                    add_rply_from_server(params[0] + " :You're not on that channel", client, "TOPIC", ERR_NOTONCHANNEL);
                    throw invalid_argument("join: user not on channel");
                }
            }
        }
        add_rply_from_server(params[0] + " :No such channel", client, "TOPIC", ERR_NOSUCHCHANNEL);
        throw invalid_argument("join: no such channel");
    }
    catch (const exception& e) {
        cout << e.what() << endl;
    }
}