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
