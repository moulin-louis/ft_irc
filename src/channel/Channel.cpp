/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armendi <armendi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 14:22:28 by armendi           #+#    #+#             */
/*   Updated: 2023/03/23 14:35:07 by armendi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel()
{
    return ;
}

Channel::Channel(string &name, Client& client)
{
    this->_name = name;
    this->_clients.push_back(client);
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
    this->_clients = rhs.getClients();
    return (*this);
}

const string    &Channel::getName() const
{
    return (this->_name);
}

const vector<Client>    &Channel::getClients() const
{
    return (this->_clients);
}

void    Channel::setName(string &name)
{
    this->_name = name;
    return ;
}

void    Channel::setClients(vector<Client> &clients)
{
    this->_clients = clients;
    return ;
}

void    Channel::addClient(Client &client)
{
    this->_clients.push_back(client);
    return ;
}

void    Channel::removeClient(Client &client)
{
    for (vector<Client>::iterator it = this->_clients.begin(); it != this->_clients.end(); it++)
    {
        if (it->getNickname() == client.getNickname())
        {
            this->_clients.erase(it);
            return ;
        }
    }
}