/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armendi <armendi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/23 14:22:28 by armendi           #+#    #+#             */
/*   Updated: 2023/03/30 15:51:32 by armendi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

/*-------------------------------CONSTRUCTORS---------------------------------*/

Channel::Channel() {
}

Channel::Channel(const string &name, const Client& client) {
    cout << "Creating new channel " << name << endl;
    this->_name = name;
    this->addClient(client);
}

Channel::Channel(const Channel &src) {
    *this = src;
}

/*---------------------------------DESTRUCTOR---------------------------------*/

Channel::~Channel() {
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

void    Channel::setTopic(const string &topic) {
    this->_topic = topic;
}

bool	Channel::user_in_chan(const Client& client) {
	for (cl_iter it = this->clients.begin(); it != this->clients.end(); ++it) {
		if (*it == client.getFd())
			return (true);
	}
	return (false);
}

void    Channel::addClient(const Client &client) {
    this->clients.push_back(client.getFd());
}

void    Channel::removeClient(const Client &client) {
    for (cl_iter it = this->clients.begin(); it != this->clients.end(); ++it) {
        if (*it == client.getFd()) {
            this->clients.erase(it);
            return ;
        }
    }
}
