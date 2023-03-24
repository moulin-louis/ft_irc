/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 14:22:28 by armendi           #+#    #+#             */
/*   Updated: 2023/03/24 15:14:46 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel()
{
    return ;
}

Channel::Channel(string &name, Client& client)
{
    cout << "Creating new channel " << name << endl;
    this->_name = name;
    this->addClient(client);
    return ;
}

Channel::Channel(const Channel &src)
{
    *this = src;
    return ;
}

Channel::~Channel()
{
    return ;
}

Channel &Channel::operator=(const Channel &rhs)
{
    this->_name = rhs.getName();
    this->clients = rhs.clients;
    return (*this);
}

const string    &Channel::getName() const
{
    return (this->_name);
}

void    Channel::setName(string &name)
{
    this->_name = name;
    return ;
}

void    Channel::addClient(Client &client)
{
    this->clients.push_back(client);
    return ;
}

void    Channel::removeClient(Client &client)
{
    for (vector<Client>::iterator it = this->clients.begin(); it != this->clients.end(); it++)
    {
        if (it->getNickname() == client.getNickname())
        {
            this->clients.erase(it);
            return ;
        }
    }
}