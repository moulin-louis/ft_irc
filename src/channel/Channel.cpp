/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armendi <armendi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 14:22:28 by armendi           #+#    #+#             */
/*   Updated: 2023/03/28 19:27:49 by armendi          ###   ########.fr       */
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

const string    &Channel::getTopic() const
{
    return (this->_topic);
}

void    Channel::setTopic(string &topic)
{
    this->_topic = topic;
    return ;
}

void    Channel::setName(string &name)
{
    this->_name = name;
    return ;
}

bool	Channel::user_in_chan(Client& client)
{
	for (cl_iter it = this->clients.begin(); it != this->clients.end(); it++) {
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
    for (cl_iter it = this->clients.begin(); it != this->clients.end(); it++)
    {
        if (it->getNickname() == client.getNickname())
        {
            this->clients.erase(it);
            return ;
        }
    }
}

void	Channel::add_cmd_client(const string& content, Client& client, Client& author, string cmd)
{
	string msg = ":" + author.getNickname() + "!" + author.getUsername() + "@" + author.getHostname() + " " + cmd + " " + this->getName() + " :" + content + endmsg;
	client.setBuff(client.getBuff() + msg);
	return ;
}

void	Channel::add_cmd_channel(const string& content, Client& author, string cmd)
{
	for (cl_iter it = this->clients.begin(); it != this->clients.end(); it++) {
		this->add_cmd_client(content, *it, author, cmd);
	}
	return ;
}

void	Channel::add_rply_from_server(string msg, Client& dest, string cmd, int code) {
	string dest_nick = dest.getNickname();
	if (dest_nick.empty())
		dest_nick = "*";
	dest_nick = " " + dest_nick;
	string result = ":localhost " + int_to_string(code) + dest_nick + " " + cmd + msg + endmsg;
	dest.setBuff(dest.getBuff() + result);
	return ;
}