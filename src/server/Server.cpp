/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loumouli <loumouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 12:52:07 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/22 17:52:21 by loumouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(const char *port, const string &password): _password(password), _port(std::strtoul(port, NULL, 10)), fd_map()
{
	this->sfd = -1;
	this->cmd_map.insert(make_pair("NICK", &Server::nick));
	this->cmd_map.insert(make_pair("USER", &Server::user));

	// this->cmd_map["NICK"] = &Server::nick;
}

Server::Server(const Server &copy): _password(copy._password), _port(copy._port), fd_map(copy.fd_map)
{
	*this = copy;
}

Server::~Server()
{
	if (this->sfd != -1)
		close(this->sfd);
}

Server &Server::operator=(const Server &assign)
{
	this->sfd = assign.sfd;
	this->fd_map = assign.fd_map;
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

void	Server::initiateSocket()
{
	this->sfd = socket( AF_INET , SOCK_STREAM, 0);
	if ( this->sfd == -1 )
		throw runtime_error(string("socket: ") + strerror(errno));
	cout << "Socket created" << endl;
	sockaddr_in	sin = {};
	sin.sin_addr.s_addr = htonl(INADDR_ANY);
	sin.sin_family = AF_INET;
	sin.sin_port = htons(this->_port);
	if ( bind(this->sfd, (sockaddr *)&sin, sizeof(sin)) == -1 )
		throw runtime_error(string("bind: ") + strerror(errno));
	cout << "bind done" << endl;
	if (listen(this->sfd, SOMAXCONN) == -1)
		throw runtime_error(string("listend: ") + strerror(errno));
	cout << "socket is listening.." << endl;
}

void	Server::parse_command( string& input, Client& client ) {
	vector<string>	result;
	size_t			pos;
	string delimiter = " ";

	while ((pos = input.find(delimiter)) != string::npos) {
		result.push_back(input.substr(0, pos));
		input.erase(0, pos + delimiter.length());
	}
	result.push_back(input);
	string cmd = result[0];
	result.erase(result.begin());
	map<string, command_function>::iterator it = this->cmd_map.find("NICK");
	if (it == this->cmd_map.end()) {
		cout << "cant find pair" << endl;
		return ;
	}
	(this->*(it->second))(result, client);
	return ;
}
