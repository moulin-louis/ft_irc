/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getter.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loumouli <loumouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 22:01:25 by loumouli          #+#    #+#             */
/*   Updated: 2023/03/28 22:01:38 by loumouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

const string &Client::getNickname() const
{
	return (this->nickname);
}

const string &Client::getUsername() const
{
	return (this->username);
}

const string &Client::getHostname() const
{
	return (this->hostname);
}

const Socket &Client::getFd() const
{
	return (this->fd);
}


string Client::getBuff() const {
	return this->buffer;
}

Mode	Client::getMode() const {
	return (this->modeUser);
}

const string &Client::getRealname() const {
	return (this->realname);
}

const string &Client::getAway() const
{
	return (this->_awayMsg);
}
