/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armendi <armendi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 19:59:56 by loumouli          #+#    #+#             */
/*   Updated: 2023/03/30 15:01:40 by armendi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*-------------------------------CONSTRUCTORS---------------------------------*/

Server::Server(const char *port, const string &password)
	: _password(password), _port(strtoul(port, NULL, 10)), _server_version("v1.0"), _server_up_date(displayTimestamp()), fd_map()
{
	cout << endl << YELLOW << "🅵 🆃" << BLINK_YELLOW << " ->" << YELLOW << "🅸 🆁 🅲" << RESET << endl;
	this->_sfd = _initiateSocket();
	this->_epfd = epoll_create1(EPOLL_CLOEXEC);
	if (this->_epfd == -1) {
		close (this->_sfd);
		throw runtime_error(string("epoll_create1: ") + strerror(errno));
	}
	this->cmd_map.insert(make_pair("NICK", &Server::nick));
	this->cmd_map.insert(make_pair("USER", &Server::user));
	this->cmd_map.insert(make_pair("JOIN", &Server::join));
	this->cmd_map.insert(make_pair("PRIVMSG", &Server::private_msg));
	this->cmd_map.insert(make_pair("PASS", &Server::pass));
	this->cmd_map.insert(make_pair("OPER", &Server::oper));
	this->cmd_map.insert(make_pair("QUIT", &Server::quit));
	this->cmd_map.insert(make_pair("MODE", &Server::mode));
	this->cmd_map.insert(make_pair("PING", &Server::ping));
	this->cmd_map.insert(make_pair("TOPIC", &Server::topic));
	this->cmd_map.insert(make_pair("PART", &Server::part));
	this->cmd_map.insert(make_pair("KICK", &Server::kick));
	this->cmd_map.insert(make_pair("kill", &Server::kill));
	this->cmd_map.insert (make_pair("LIST", &Server::list));
	this->cmd_map.insert(make_pair("WHO", &Server::who));
}

Socket	Server::_initiateSocket() {
	sockaddr_in	sin = {};
	int			opt = 1;
	Socket 		sfd;
	char		hostname[NI_MAXHOST];

	string temp;
	cout << PURPLE << "Please input your admin password" << RESET << endl;
	cin >> temp;
	cout << PURPLE << "your password is [" << temp << "]" << RESET << endl;
	this->admin_pass = temp;
	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(this->_port);
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd < 0)
		throw runtime_error(string("socket: ") + strerror(errno));
	cout << GREEN << "Socket created" << RESET << endl;
	if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw runtime_error(string("setsockopt: ") + strerror(errno));
	if (fcntl(sfd, F_SETFL, O_NONBLOCK) == -1)
		throw runtime_error(string("fcntl: ") + strerror(errno));
	if (bind(sfd, (sockaddr *)&sin, sizeof(sin)) == -1 )
		throw runtime_error(string("bind: ") + strerror(errno));
	cout << GREEN << "Bind done" << RESET << endl;
	if (listen(sfd, SOMAXCONN) < 0)
		throw runtime_error(string("listen: ") + strerror(errno));
	if (getnameinfo(reinterpret_cast<const sockaddr *>(&sin), sizeof(sin), hostname, sizeof(hostname), NULL, 0, 0) != 0)
	{
		this->_server_name = "unknown";
	}
	else
	{
		this->_server_name = hostname;
	}
	cout << GREEN << "Socket is listening, server address is: " << this->_server_name << RESET << endl;
	return (sfd);
}
