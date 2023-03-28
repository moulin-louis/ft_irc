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
	typedef vector<Client>::iterator cl_iter;
    private:
        string _name;
        string _topic;
    public:
        Channel();
        Channel(string &name, Client& client);
        Channel(const Channel &src);
        ~Channel();
        Channel &operator=(const Channel &rhs);

        vector<Client> clients;

        const string    &getName() const;
        const string    &getTopic() const;
        void            setTopic(string &topic);
        void            setName(string &name);

    //checks
        bool	user_in_chan(Client& client);
       
    //sens messages
        void	add_cmd_client(const string& content, Client& client, Client& author, string cmd);
        void	add_cmd_channel(const string& content, Client& author, string cmd);
        void    add_rply_from_server(string msg, Client& dest, string cmd, int code);

    //actions
        void    addClient(Client &client);
        void    removeClient(Client &client);
        void    removeClient(string &nick);
        void	notify_clients(Channel& channel, Client& client, string cmd);
        void    process_topic_cmd(vector <string> params, Client& client);
};

#endif