/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loumouli <loumouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 12:52:07 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/22 16:37:50 by loumouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server(const char *port, const string &password): _password(password), _port(std::strtoul(port, NULL, 10)), fd_map()
{
	this->sfd = -1;
	this->cmd_map.insert("NICK", &Server::nick);
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

// Client*	Server::register_connection(string& entry)
// {
// 	Client *newUser;
// 	string	nick;
// 	string	username;
// 	size_t	begin = 0;
// 	size_t	len = 0;

// 	begin = entry.find("NICK") + 5;
// 	len = entry.find('\n') - begin;
// 	nick = entry.substr(begin, len);
// 	begin = entry.find("USER") + 5;
// 	len = entry.find('$') - begin;
// 	username = entry.substr(begin, len);
// 	newUser = new Client(nick, username);
// 	return (newUser);
// }

bool is_valid_nickname(string &nickname)
{
	if (nickname.size() > 9)
		return (false);
	//check if nickname is already taken by another client
	return (true);
}

string	Server::msg_welcome(Client& client)
{
	string msg;
	msg = string(":localhost") + string("001") + client.nickname + string(" :Welcome to the Internet Relay Network ") + client.nickname + string("!") + client.username + string("@") + client.hostname;
	return (msg);
}

string	Server::msg_invalid_nick(Client& client)
{
	string msg;
	msg = string(":localhost") + string("001") + '*' + client.nickname + string(" :Nickname is already in use");
	return (msg);
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

