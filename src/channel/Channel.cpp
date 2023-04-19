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
#include "typedef.hpp"

/*-------------------------------CONSTRUCTORS---------------------------------*/

Channel::Channel() {
}

Channel::Channel(const string &name, const Client& client) {
    cout << "Creating new channel " << name << endl;
    this->_name = name;
	this->_modeChannel = 0;
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
	this->_modeChannel = rhs._modeChannel;
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
	cl_iter it = find(this->clients.begin(), this->clients.end(), client.getFd());
	if (it == this->clients.end()) {
		return (false);
	}
	return (true);
}

void    Channel::addClient(const Client &client) {
    this->clients.push_back(client.getFd());
}

void    Channel::removeClient(const Client &client) {
	this->clients.erase(find(this->clients.begin(), this->clients.end(), client.getFd()));
}

Mode    Channel::getMode() const
{
	return (this->_modeChannel);
}

void    Channel::setMode(Mode input)
{
	this->_modeChannel |= input;
}

void    Channel::unSetMode(Mode input)
{
	this->_modeChannel &= ~input;
}
