/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topic.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armendi <armendi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 16:46:02 by armendi           #+#    #+#             */
/*   Updated: 2023/03/28 17:31:25 by armendi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "Server.hpp"

void    Channel::process_topic_cmd(vector <string> params, Client& client)
{
    if (params.size() == 2)
    {
        this->setTopic(params[1]);
        this->add_cmd_channel(params[1], client, "TOPIC");
    }
    else
    {
        if (this->getTopic().empty())
            this->add_rply_from_server(this->getName() + ":No topic is set", client, "TOPIC", RPL_NOTOPIC);
        else
            this->add_rply_from_server(this->getName() + " :" + this->getTopic(), client, "TOPIC", RPL_TOPIC);
    }
    return ;
}

void	Server::topic( vector<string> params, Client& client )
{
    try {
        if (params.size() != 1)
        {
            add_rply_from_server(":Not enough parameters", client, "TOPIC", ERR_NEEDMOREPARAMS);
            throw invalid_argument("join: invalid number of parameters");
        }
        if (params[0][0] != '#')
        {
            add_rply_from_server(params[0] + " :Invalid channel name", client, "TOPIC", ERR_NOSUCHCHANNEL);
            throw invalid_argument("join: invalid channel name");
        }
        for (chan_iter it = chan_map.begin(); it != chan_map.end(); it++)
        {
            if (it->getName() == params[0])
            {
                if (it->user_in_chan(client))
                {
                    it->process_topic_cmd(params, client);
                    return ;
                }
                else
                {
                    add_rply_from_server(params[0] + " :You're not on that channel", client, "TOPIC", ERR_NOTONCHANNEL);
                    throw invalid_argument("join: user not on channel");
                }
            }
        }
    }
    catch (const exception& e) {
        cout << e.what() << endl;
    }
    return ;
}