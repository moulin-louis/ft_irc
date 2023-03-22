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

bool stop = false;

void handler(int)
{
	std::cout << "Signal received" << std::endl;
	stop = true;
}

Server::Server(const char *port, const string &password)
	: _password(password), _port(std::strtoul(port, NULL, 10)), fd_map()
{
	this->_sfd = -1;
	this->_sfd = _initiateSocket();
	this->_epfd = epoll_create1(EPOLL_CLOEXEC);
	if (this->_epfd == -1)
	{
		close (this->_sfd);
		throw runtime_error(string("epoll_create1: ") + strerror(errno));
	}
//	this->cmd_map.insert("NICK", &Server::nick);
}

Server::Server(const Server &copy): _password(copy._password), _port(copy._port), fd_map(copy.fd_map)
{
	*this = copy;
}

Server::~Server()
{
	if (this->_sfd >= 0)
		close(this->_sfd);
	if (this->_epfd >= 0)
		close(this->_epfd);
}

Server &Server::operator=(const Server &assign)
{
	this->_sfd = assign._sfd;
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

Socket	Server::_initiateSocket() const
{
	sockaddr_in	sin = {};
	int			opt = 1;
	Socket 		sfd;

	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(this->_port);
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd < 0)
		throw runtime_error(string("socket: ") + strerror(errno));
	cout << "Socket created" << endl;
	if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
		throw runtime_error(string("setsockopt: ") + strerror(errno));
	if (fcntl(sfd, F_SETFL, O_NONBLOCK) == -1)
		throw runtime_error(string("fcntl: ") + strerror(errno));
	if (bind(sfd, (sockaddr *)&sin, sizeof(sin)) == -1 )
		throw runtime_error(string("bind: ") + strerror(errno));
	cout << "bind done" << endl;
	if (listen(sfd, SOMAXCONN) < 0)
		throw runtime_error(string("listend: ") + strerror(errno));
	cout << "socket is listening.." << endl;
	return (sfd);
}

void	Server::run()
{
	epoll_event ev = {};
	int nfds;
	int n;

	ev.events = EPOLLIN;
	ev.data.fd = this->_sfd;
	if (epoll_ctl(this->_epfd, EPOLL_CTL_ADD, this->_sfd, &ev) == -1)
		throw runtime_error(string("epoll_ctl: ") + strerror(errno));
	signal(SIGINT, handler);
	while (!stop)
	{
		nfds = epoll_wait(this->_epfd, this->_events, MAX_EVENTS, -1);
		if (nfds == -1)
		{
			if (errno == EINTR)
				continue;
			throw runtime_error(string("epoll_wait: ") + strerror(errno));
		}
		for (n = 0; n < nfds; ++n)
		{
			if (this->_events[n].events & EPOLLIN)
			{
				if (this->_events[n].data.fd == this->_sfd)
				{
					// mettre ca ailleurs et faire une fonction
					std::cout << "new connection" << std::endl;
					sockaddr csin = {};
					socklen_t crecsize = sizeof(csin);
					Socket csock = accept(this->_sfd, &csin, &crecsize);
					if ( csock == -1 )
						{
							close(this->_sfd);
							throw runtime_error(string("accept: ") + strerror(errno));
						}
					this->fd_map.insert( pair<Socket, Client>(csock, Client()) );
				}
				else
				{
					// Gérer les event autres que la connection
					std::cout << "new event" << std::endl;
				}
			}
			else
			{
				if (this->_events[n].events & (EPOLLHUP | EPOLLRDHUP))
				{
					std::cout << "client disconnected" << std::endl;
				}
			}
		}
	}
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

