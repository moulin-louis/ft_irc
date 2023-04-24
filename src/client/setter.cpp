/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setter.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loumouli <loumouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 22:02:03 by loumouli          #+#    #+#             */
/*   Updated: 2023/03/28 22:02:11 by loumouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

void Client::setNickname(const string &nickname_input)
{
	this->nickname = nickname_input;
}

void Client::setUsername(const string &username_input)
{
	this->username = username_input;
}

void Client::setRealname(const string &rlname_input)
{
	this->realname = rlname_input;
}

void Client::setHostname(const string &hostname_input)
{
	this->hostname = hostname_input;
}

void Client::setFd(const Socket &fd_input)
{
	this->fd = fd_input;
}

void Client::setBuff(const string &buffer_input) {
	this->buffer = buffer_input;
}

void Client::setMode(Mode input)
{
	this->modeUser |= input;
}

void Client::unSetMode(Mode input)
{
	this->modeUser &= ~input;
}

void Client::setAway(const string& msg)
{
	this->_awayMsg = msg;
}
