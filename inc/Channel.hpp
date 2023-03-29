/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armendi <armendi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 16:33:26 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/28 17:28:49 by armendi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef CHANNEL_HPP
# define CHANNEL_HPP

# include <string>
# include <iostream>
# include "irc.hpp"
# include "Server.hpp"

class Server;

class Channel
{
    private:
        string _name;
        string _topic;
    public:
        Channel();
        Channel(string &name, Client& client);
        Channel(const Channel &src);
        ~Channel();
        Channel &operator=(const Channel &rhs);

		//vector of client's nickname
        vector<Socket> clients;

        const string    &getName() const;
        const string    &getTopic() const;
        void            setTopic(string &topic);
        void            setName(string &name);

    //checks
        bool	user_in_chan(Client& client);

    //actions
        void    addClient(Client &client);
        void    process_topic_cmd(vector <string> params, Client& client, Server* server);
};

#endif