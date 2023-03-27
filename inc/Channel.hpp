/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 16:33:26 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/27 16:27:02 by mpignet          ###   ########.fr       */
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

        vector<Client> clients;

        const string    &getName() const;
        void            setName(string &name);

    //checks
        bool	user_in_chan(Client& client);
       
    //sens messages
        void	add_cmd_client(string& content, Client& client, Client& author, string cmd);
        void	add_cmd_channel(string& content, Client& author, string cmd);

    //actions
        void    addClient(Client &client);
        void    removeClient(Client &client);
        void    removeClient(string &nick);
        void	notify_clients(Channel& channel, Client& client, string cmd);
};

#endif