/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 11:43:47 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/22 11:45:24 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

Client &parse_entry(string &entry);

Client::Client(void)
{
	return ;
}

Client::Client(string &nick, string &user)
{
	this->nickname = nick;
	this->username = user;
	return ;
}

Client::Client(const Client &copy)
{
	*this = copy;
	return ;
}

Client::~Client()
{

}

Client &Client::operator=(const Client &assign)
{
	(void) assign;
	return (*this);
}

const string &Client::getNickname() const
{
	return (this->nickname);
}

const string &Client::getUsername() const
{
	return (this->username);
}

const string &Client::getRealname() const
{
	return (this->realname);
}

const Socket &Client::getFd() const
{
	return (this->fd);
}

const sockaddr_in &Client::getSin() const
{
	return (this->sin);
}

void Client::setNickname(const string &nickname)
{
	this->nickname = nickname;
}

void Client::setUsername(const string &username)
{
	this->username = username;
}

void Client::setRealname(const string &realname)
{
	this->realname = realname;
}

void Client::setFd(const Socket &fd)
{
	this->fd = fd;
}

void Client::setSin(const sockaddr_in &sin)
{
	this->sin = sin;
}
