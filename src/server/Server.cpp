/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 12:52:07 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/22 14:28:11 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

#define ERR 001

Server::Server(const string &port, const string &password): _port(port), _password(password), _port_uint(atoi(port.c_str()))
{

}

Server::Server(const Server &copy): _port(copy._port), _password(copy._password), _port_uint(copy._port_uint)
{
	*this = copy;
}

Server::~Server()
{

}

Server &Server::operator=(const Server &assign)
{
	(void) assign;
	return (*this);
}

const string &Server::getPort() const
{
	return (this->_port);
}

const string &Server::getPassword() const
{
	return (this->_password);
}

uint16_t Server::getPort_uint() const
{
	return (this->_port_uint);
}

Client&	Server::register_connection(string& entry)
{
	string	nick;
	string	username;
	size_t	begin = 0;
	size_t	len = 0;

	begin = entry.find("NICK") + 5;
	len = entry.find('\n') - begin;
	nick = entry.substr(begin, len);
	begin = entry.find("USER") + 5;
	len = entry.find('$') - begin;
	username = entry.substr(begin, len);
	Client newUser(nick, username);
	return (newUser);
}

bool is_valid_nickname(string &nickname)
{
	if (nickname.size() > 9)
		return (false);
	//check if nickname is already taken by another client
	return (true);
}

string	Server::msg_refuse_connection(Client& client)
{	
	string msg;
	msg = ":localhost" + ERR + '*' + client.nickname + " :Nickname is already in use";
	return (msg);
}

string	Server::msg_accept_connection(Client& client)
{	
	string msg;
	msg = ":localhost" + ERR + client.nickname + " :Welcome to the Internet Relay Network " + client.nickname + "!" + client.username + "@" + client.hostname;
	return (msg);
}