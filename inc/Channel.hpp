/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 16:33:26 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/29 15:33:41 by mpignet          ###   ########.fr       */
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

		//vector of client's fd
        vector<Socket> clients;

        const string    &getName() const;
        const string    &getTopic() const;
        void            setTopic(string &topic);
        void            setName(string &name);

    //checks
        bool	user_in_chan(Client& client);

    //actions
        void    addClient(Client &client);
        void    removeClient(Client &client);
};

#endif