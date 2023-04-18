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

Client::Client(void) : fd(), passwd_provided(), isRegistered(), isAway(), isOperator() {
	memset(static_cast<void *>(this), 0, sizeof(Client));
	this->setNickname("*");
	this->modeUser = 0;
	this->isLeaving = false;
	asTriedNickname = false;
}

Client::Client(const Client &copy)
		: fd(), passwd_provided(), isRegistered(), isAway(), isOperator(), asTriedNickname() {
	memset(static_cast<void *>(this), 0, sizeof(Client));
	this->nickname = copy.nickname;
	this->modeUser = copy.modeUser;
	this->isLeaving = copy.isLeaving;
	*this = copy;
}

Client::~Client() {
	if (this->fd)
		close(this->fd);
}

Client &Client::operator=(const Client &assign) {
	(void) assign;
	this->fd = assign.fd;
	this->modeUser = assign.modeUser;
	return (*this);
}

void Client::clearBuff() {
	if (!this->buffer.empty())
		this->buffer.clear();
}

