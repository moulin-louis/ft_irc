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

bool stop = false;

void handler(int)
{
	cout << YELLOW << "Signal received" << RESET << endl;
	stop = true;
}

Server::Server(const char *port, const string &password)
	: _password(password), _port(strtoul(port, NULL, 10)), fd_map()
{
	this->_sfd = _initiateSocket();
	this->_epfd = epoll_create1(EPOLL_CLOEXEC);
	if (this->_epfd == -1)
	{
		close (this->_sfd);
		throw runtime_error(string("epoll_create1: ") + strerror(errno));
	}
	this->cmd_map.insert(make_pair("NICK", &Server::nick));
	this->cmd_map.insert(make_pair("USER", &Server::user));
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

Client&	Server::find_user(string nick)
{
	for (map<int, Client>::iterator it = this->fd_map.begin(); it != this->fd_map.end(); it++)
	{
		if (it->second.getNickname() == nick)
			return it->second;
	}
	throw runtime_error("User not found");
}

void	Server::send_client(string& msg, Client& clt_to)
{
	string msg_to_send = ":" + clt_to.getNickname() + "!" + clt_to.getUsername() + "@" + clt_to.getHostname() + " " + msg;
	send(clt_to.getFd(), msg.c_str(), msg.size(), MSG_DONTWAIT);
	return ;
}

void	Server::send_client(string& msg, Client& clt_from, Client& clt_to)
{
	string msg_to_send = ":" + clt_from.getNickname() + "!" + clt_from.getUsername() + "@" + clt_from.getHostname() + " PRIVMSG " + clt_to.getNickname() + " :" + msg;
	send(clt_to.getFd(), msg_to_send.c_str(), msg.size(), MSG_DONTWAIT);
	return ;
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
		if (nfds == -1) {
			if (errno == EINTR)
				continue;
			throw runtime_error(string("epoll_wait: ") + strerror(errno));
		}
		for (n = 0; n < nfds; ++n) {
			Socket temp_fd = this->_events[n].data.fd;
			if (this->_events[n].events & EPOLLIN) {
				if (temp_fd == this->_sfd)
					this->accept_client();
				else {
					if (this->fd_map.find(temp_fd) != this->fd_map.end()) {
						this->process_input(temp_fd);
					}
				}
			}
			else {
				if (this->_events[n].events & EPOLLHUP || this->_events[n].events & EPOLLRDHUP)
					this->disconect_client(n);
			}
		}
	}
	cout << RED << "Stoping the server" << RESET << endl;
}

void	Server::accept_client( void ) {
	epoll_event ev = {};
	sockaddr csin = {};
	socklen_t crecsize = sizeof(csin);

	Socket csock = accept(this->_sfd, &csin, &crecsize);
	if ( csock == -1 ) {
		close(this->_sfd);
		throw runtime_error(string("accept: ") + strerror(errno));
	}
	cout << GREEN << "New connection" << RESET << endl;
	ev.events = EPOLLIN | EPOLLOUT;
	ev.data.fd = csock;
	this->fd_map.insert(make_pair(csock, Client()) );
	this->fd_map[csock].setFd(csock);
	epoll_ctl(this->_epfd, EPOLL_CTL_ADD, csock, &ev);
}

void	Server::disconect_client( int pos_events ) {
	epoll_event ev = {};

	epoll_ctl(this->_epfd, EPOLL_CTL_DEL, this->_events[pos_events].data.fd, &ev);
	this->fd_map.erase( this->fd_map.find(this->_events[pos_events].data.fd));
	cout << GREEN << "client disconnected" << RESET << endl;
}

void	Server::process_input(Socket fd ) {
	string temp = this->received_data_from_client(fd);
	if (temp.empty()) {
		return ;
	}
	cout << temp << endl;	
	this->parse_command(temp, this->fd_map[fd]);
}

string	Server::received_data_from_client(Socket fd) {
	string result;
	result.resize(512);
	int ret_val = recv(fd, (void *)result.c_str(), 512, 0);
	if (ret_val == -1 ) {
		throw invalid_argument(string("Recv: ") + strerror(errno));
	}
	if (ret_val == 0) {
		return (result.clear(), result);
	}
	return result;
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
	map<string, command_function>::iterator it = this->cmd_map.find(cmd);
	if (it == this->cmd_map.end()) {
		cout << "cant find pair" << endl;
		return ;
	}
	(this->*(it->second))(result, client);
	return ;
}
