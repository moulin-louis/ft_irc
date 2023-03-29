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
#include <cstdlib>

Client::Client(void) {
	cout << "new client created" << endl;
	memset((void*)this, 0, sizeof(Client));
	return ;
}

Client::Client(string &nick, string &user) {
	cout << "new client created" << endl;
	memset((void*)this, 0, sizeof(Client));
	this->nickname = nick;
	this->username = user;
	return ;
}

Client::Client(const Client &copy) {
	cout << "new client created" << endl;
	memset((void*)this, 0, sizeof(Client));
	*this = copy;
	return ;
}

Client::~Client() {
	if (this->fd)
		close(this->fd);
}

Client &Client::operator=(const Client &assign) {
	memcpy((void*)this, &assign, sizeof(Client));
	return (*this);
}

void Client::clearBuff() {
	this->buffer.clear();
}

void Client::setMode( int idx, Mode input) {
	this->mode[idx] = input;
}