/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armendi <armendi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 16:33:26 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/23 14:35:22 by armendi          ###   ########.fr       */
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
        vector<Client> _clients;
    public:
        Channel();
        Channel(string &name, Client& client);
        Channel(const Channel &src);
        ~Channel();
        Channel &operator=(const Channel &rhs);

        const string            &getName() const;
        const vector<Client>    &getClients() const;

        void    setName(string &name);
        void    setClients(vector<Client> &clients);

        void    addClient(Client &client);
        void    removeClient(Client &client);
        void    removeClient(string &nick);
};

#endif