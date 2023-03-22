/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 12:52:07 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/22 16:42:25 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(const char *port, const string &password): _password(password), _port(std::strtoul(port, NULL, 10)), fd_map()
{
	this->cmd_map.insert("NICK", &Server::nick);
	return ;
}
Server::Server(const Server &copy): _password(copy._password), _port(copy._port), fd_map(copy.fd_map)
{
	*this = copy;
}

Server::~Server()
{
	return ;
}

Server &Server::operator=(const Server &assign)
{
	(void) assign;
	return (*this);
}

const string &Server::getPassword() const
{
	return (this->_password);
}

const uint16_t &Server::getPort() const
{
	return (this->_port);
}

void	Server::send_client(string& msg, Client& client)
{
	(void)msg;
	(void)client;
	return ;
}

void	Server::parse_command( string& input ) {
	vector<string>	result;
	string delimiter = " ";
	size_t			pos;

	while ((pos = input.find(delimiter)) != string::npos) {
		result.push_back(input.substr(0, pos));
		input.erase(0, pos + delimiter.length());
	}
	return ;
}

	
