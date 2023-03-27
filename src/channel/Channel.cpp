/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 14:22:28 by armendi           #+#    #+#             */
/*   Updated: 2023/03/27 17:36:23 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/*-------------------------------CONSTRUCTORS---------------------------------*/

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

/*---------------------------------DESTRUCTOR---------------------------------*/

Channel::~Channel()
{
    return ;
}

/*---------------------------------OPERATORS----------------------------------*/


Channel &Channel::operator=(const Channel &rhs)
{
    this->_name = rhs.getName();
    this->clients = rhs.clients;
    return (*this);
}

/*------------------------------MEMBER FUNCTIONS------------------------------*/

const string    &Channel::getName() const
{
    return (this->_name);
}

void    Channel::setName(string &name)
{
    this->_name = name;
    return ;
}

bool	Channel::user_in_chan(Client& client)
{
	for (vector<Client>::iterator it = this->clients.begin(); it != this->clients.end(); it++) {
		if (it->getNickname() == client.getNickname())
			return (true);
	}
	return (false);
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

void	Channel::add_cmd_client(string& content, Client& client, Client& author, string cmd)
{
	string msg = ":" + author.getNickname() + "!" + author.getUsername() + "@" + author.getHostname() + " " + cmd + " :" + content + endmsg;
	client.setBuff(client.getBuff() + msg);
	return ;
}

void	Channel::add_cmd_channel(string& content, Client& author, string cmd)
{
	for (vector<Client>::iterator it = this->clients.begin(); it != this->clients.end(); it++) {
		this->add_cmd_client(content, *it, author, cmd);
	}
	return ;
}
