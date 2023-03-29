/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loumouli <loumouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 14:22:28 by armendi           #+#    #+#             */
/*   Updated: 2023/03/28 22:04:33 by loumouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/*-------------------------------CONSTRUCTORS---------------------------------*/

Channel::Channel() {
    return ;
}

Channel::Channel(string &name, Client& client) {
    cout << "Creating new channel " << name << endl;
    this->_name = name;
    this->addClient(client);
    return ;
}

Channel::Channel(const Channel &src) {
    *this = src;
    return ;
}

/*---------------------------------DESTRUCTOR---------------------------------*/

Channel::~Channel() {
    return ;
}

/*---------------------------------OPERATORS----------------------------------*/


Channel &Channel::operator=(const Channel &rhs) {
    this->_name = rhs.getName();
    this->clients = rhs.clients;
    return (*this);
}

/*------------------------------MEMBER FUNCTIONS------------------------------*/

const string    &Channel::getName() const {
    return (this->_name);
}

const string    &Channel::getTopic() const {
    return (this->_topic);
}

void    Channel::setTopic(string &topic) {
    this->_topic = topic;
    return ;
}

void    Channel::setName(string &name) {
    this->_name = name;
    return ;
}

bool	Channel::user_in_chan(Client& client) {
	for (cl_iter it = this->clients.begin(); it != this->clients.end(); it++) {
		if (*it == client.getFd())
			return (true);
	}
	return (false);
}

void    Channel::addClient(Client &client) {
    this->clients.push_back(client.getFd());
    return ;
}
