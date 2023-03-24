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
	this->passwd_provided = false;
	this->isRegistered = false;
	this->isAway = false;
	this->isOperator = false;
	return ;
}

Client::Client(string &nick, string &user)
{
	this->passwd_provided = false;
	this->isRegistered = false;
	this->isAway = false;
	this->isOperator = false;
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

const string &Client::getHostname() const
{
	return (this->hostname);
}

const Socket &Client::getFd() const
{
	return (this->fd);
}

const sockaddr_in &Client::getSin() const
{
	return (this->sin);
}

string Client::getBuff() const {
	return this->buffer;
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

void Client::setHostname(const string &hostname)
{
	this->hostname = hostname;
}

void Client::setFd(const Socket &fd)
{
	this->fd = fd;
}

void Client::setSin(const sockaddr_in &sin)
{
	this->sin = sin;
}

void Client::clearBuff() {
	this->buffer.clear();
}

void Client::setBuff(const string &buffer) {
	this->buffer = buffer;
}