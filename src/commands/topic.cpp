/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 16:46:02 by armendi           #+#    #+#             */
/*   Updated: 2023/04/16 16:24:54 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void    Server::process_topic_cmd( const vector <string>& params, Client& client, Channel& chan)
{
    if (params.size() == 2) {
        chan.setTopic(params[1].substr(1));
        //this->add_rply_from_server(" :" + chan.getName() + " :" + chan.getTopic(), client, "TOPIC", RPL_TOPIC);
        this->notify_chan(chan, params[1].substr(1), "TOPIC", client);
	}
    else {
        if (chan.getTopic().empty())
            this->add_rply_from_server(chan.getName() + ":No topic set", client, "", RPL_NOTOPIC);
        else
            this->add_rply_from_server(chan.getName() + " :" + chan.getTopic(), client, "", RPL_TOPIC);
    }
}

void	Server::topic( const vector<string>& params, Client& client )
{
    for (size_t i = 0; i < params.size(); i++)
        cout << "params[" << i << "] = " << params[i] << endl;
    try {
        if (params.size() == 0) {
            add_rply_from_server(":Not enough parameters", client, "TOPIC", ERR_NEEDMOREPARAMS);
            throw invalid_argument("topic: invalid number of parameters");
        }
        // if (params[0][0] != '#') {
        //     add_rply_from_server(params[0] + " :Invalid channel name", client, "TOPIC", ERR_NOSUCHCHANNEL);
        //     throw invalid_argument("topic: invalid channel name");
        // }
        bool chan_found = false;
        for (chan_iter it = chan_vec.begin(); it != chan_vec.end(); ++it) {
            if (it->getName() == "#" + params[1])
            {
                chan_found = true;
                if (it->user_in_chan(client))
                {
                    this->process_topic_cmd(params, client, *it);
                    return ;
                }
                else
                {
                    add_rply_from_server(params[0] + " :You're not on that channel", client, "TOPIC", ERR_NOTONCHANNEL);
                    throw invalid_argument("topic: user not on channel");
                }
            }
        }
        if (chan_found == false) {
            //use params[1] as topic for chan in params[0]
            for(chan_iter it = chan_vec.begin(); it != chan_vec.end(); ++it) {
                if (it->getName() == params[0]) {
                    it->setTopic(params[1].substr(1));
                    this->notify_chan(*it, it->getTopic(), "TOPIC", client);
                    return ;
                }
            }
            return ;
        }
        // add_rply_from_server(params[0] + " :No such channel", client, "TOPIC", ERR_NOSUCHCHANNEL);
        // throw invalid_argument("topic: no such channel");
    }
    catch (const exception& e) {
        cout << e.what() << endl;
    }
    // (void) params;
    // (void) client;
}