/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_server.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loumouli <loumouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 19:59:56 by loumouli          #+#    #+#             */
/*   Updated: 2023/03/29 16:05:19 by loumouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*-------------------------------CONSTRUCTORS---------------------------------*/

Server::Server(const char *port, const string &password)
	: _password(password), _port(strtoul(port, NULL, 10)), _server_name("ft_irc.net"), _server_version("v1.0"), _server_up_date(displayTimestamp()), fd_map()
{
	std::cout << std::endl << YELLOW << "🅵 🆃" << BLINK_YELLOW << " ->" << YELLOW << "🅸 🆁 🅲" << RESET << std::endl;
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
	this->cmd_map.insert(make_pair("kill", &Server::kill));
	this->cmd_map.insert (make_pair("LIST", &Server::list));
}

Socket	Server::_initiateSocket() {
	sockaddr_in	sin = {};
	int			opt = 1;
	Socket 		sfd;

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
	cout << GREEN << "Socket is listening.." << RESET << endl;
	return (sfd);
}
