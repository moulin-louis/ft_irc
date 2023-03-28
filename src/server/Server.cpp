/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loumouli <loumouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 12:52:07 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/28 11:57:24 by loumouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool stop = false;

void handler(int)
{
	cout << YELLOW << "Signal received" << RESET << endl;
	stop = true;
}

static int epoll_ctl_add(int epfd, int fd, uint32_t events)
{
	struct epoll_event ev = {};
	bzero(&ev.data, sizeof(ev.data));
	ev.events = events;
	ev.data.fd = fd;
	return (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev));
}

void	sendMessage(Client &client, const std::string& message)
{
	send(client.getFd(), message.c_str(), message.length(), 0);
}

/*-------------------------------CONSTRUCTORS---------------------------------*/

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
	this->cmd_map.insert(make_pair("JOIN", &Server::join));
	this->cmd_map.insert(make_pair("PRIVMSG", &Server::private_msg));
	this->cmd_map.insert(make_pair("PASS", &Server::pass));
	this->cmd_map.insert(make_pair("OPER", &Server::oper));
	this->cmd_map.insert(make_pair("QUIT", &Server::quit));
}

/*---------------------------------DESTRUCTOR---------------------------------*/

Server::~Server()
{
	if (this->_sfd >= 0)
		close(this->_sfd);
	if (this->_epfd >= 0)
		close(this->_epfd);
}


/*------------------------------MEMBER FUNCTIONS------------------------------*/

Client&	Server::find_user(string nick, Client client)
{
	for (map<int, Client>::iterator it = this->fd_map.begin(); it != this->fd_map.end(); it++)
	{
		if (it->second.getNickname() == nick)
			return it->second;
	}
	add_rply_from_server(nick + " :No such nick/channel", client, "NICK", ERR_NOSUCHNICK);
	throw runtime_error("User not found");
}

Channel&	Server::find_channel(string name, Client client)
{
	for (vector<Channel>::iterator it = this->chan_map.begin(); it != this->chan_map.end(); it++)
	{
		if (it->getName() == name)
			return *it;
	}
	add_rply_from_server(name + " :No such nick/channel", client, "JOIN", ERR_NOSUCHCHANNEL);
	throw runtime_error("User not found");
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

	if (epoll_ctl_add(this->_epfd, this->_sfd, EPOLLIN) == -1)
		throw runtime_error(string("epoll_ctl: ") + strerror(errno));
	signal(SIGINT, handler);
	while (!stop)
	{
		nfds = epoll_wait(this->_epfd, this->_events, 10, -1);
		if (nfds == -1)
		{
			if (errno == EINTR)
				continue;
			throw runtime_error(string("epoll_wait: ") + strerror(errno));
		}
		for (int n = 0; n < nfds; ++n)
		{
			ev = this->_events[n];
			if (ev.events & EPOLLIN)
			{
				if (ev.data.fd == this->_sfd)
					this->_accept_client();
				else
				{
					this->process_input(ev.data.fd);
				}
			}
			if (ev.events & (EPOLLHUP | EPOLLRDHUP))
				this->_disconect_client(ev.data.fd);
		}
	}
	cout << RED << "Server stopped" << RESET << endl;
}

void	Server::_accept_client( void ) {
	sockaddr csin = {};
	socklen_t crecsize = sizeof(csin);
	Client	temp;

	Socket csock = accept(this->_sfd, (struct sockaddr *)&csin, &crecsize);
	if (csock < 0)
	{
		throw runtime_error(string("accept: ") + strerror(errno));
	}
	cout << GREEN << "New connection" << RESET << endl;
	this->fd_map.insert(make_pair(csock, Client()) );
	char hostname[NI_MAXHOST];
	if (getnameinfo(&csin, sizeof(csin), hostname, sizeof(hostname), NULL, 0, 0) != 0)
		this->fd_map[csock].setHostname("unknown");
	else
		this->fd_map[csock].setHostname(hostname);
	std::cout << "Server: got connection from " << this->fd_map[csock].getHostname() << std::endl;
	this->fd_map[csock].setFd(csock);
	epoll_ctl_add(this->_epfd, this->fd_map[csock].getFd(), EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLRDHUP);
}

void	Server::_disconect_client( Socket fd )
{
	client_iter it = this->fd_map.find(fd);
	epoll_ctl(this->_epfd, EPOLL_CTL_DEL, fd, NULL);
	close(fd);
	if ( this->fd_map.erase(fd) == 0 )
	{
		cout << RED << "problem deleting client from database" << RESET << endl;
	}
	if (!it->second.getNickname().empty())
		std::cout << GREEN << it->second.getNickname() << " closed the connection" << RESET << std::endl;
	else
		std::cout << GREEN << it->second.getHostname() << " closed the connection" << RESET << std::endl;
}

string	Server::received_data_from_client(Socket fd)
{
	string result;
	result.resize(512);
	
	int ret_val = recv(fd, (void *)result.c_str(), 512, 0);
	if (ret_val == -1 )
	{
		if ( errno == ECONNRESET )
		{
			this->_disconect_client(fd);
			return (result.clear(), result);
		}
		throw invalid_argument(string("Recv: ") + strerror(errno));
	}
	if (ret_val == 0)
	{
		cout << YELLOW << "nothing received" << endl;
		return (result.clear(), result);
	}
	cout << YELLOW << ret_val << " bytes received" << RESET << endl;
	result.resize(ret_val + 1);
	return (result);
}

#define MSG ":" + client.getHostname() + " 001 " + client.getNickname() + " Welcome to the Internet Relay Network TNO " + endmsg

void	Server::process_input(Socket fd )
{
	//string temp = this->received_data_from_client(fd);
	//if (temp.empty())
	//{
	//	return ;
	//}
	//while (1)
	//{
	//	if (temp.find(endmsg) == string::npos)
	//	{
	//		break ;
	//	}
	//	string tok = temp.substr(0, temp.find(endmsg));
	//	parse_command(tok, this->fd_map[fd]);
	//	temp.erase(0, temp.find(endmsg) + 2);
	//}

	client_iter 	it = this->fd_map.find(fd);
	Client			&client = it->second;
	int 			byte_count;
	char			buf[2048];

	byte_count = recv(fd, buf, sizeof(buf), 0);
	buf[byte_count] = '\0';
	cout << client.getHostname() << " : " << buf << endl;
	client.getBuff().append(buf);
	if (client.getBuff()[client.getBuff().length() - 1] == '\n')
	{
		std::cout << "Command received from : " << client.getHostname() << std::endl;
		std::cout << CYAN << client.getBuff() << RESET;
	}
	sendMessage(client, MSG);
	parse_command((const string)client.getBuff(), client);
}

void	Server::parse_command(basic_string<char> input, Client& client )
{
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
		return ;
	}
	(this->*(it->second))(result, client);
}

void	Server::add_cmd_client(string& content, Client& client, Client& author, string cmd)
{
	string msg = ":" + author.getNickname() + "!" + author.getUsername() + "@" + author.getHostname() + " " + cmd + " :" + content + endmsg;
	client.setBuff(client.getBuff() + msg);
	return ;
}

void	Server::add_rply_from_server(string msg, Client& dest, string cmd, int code)
{
	string dest_nick = dest.getNickname();
	if (dest_nick.empty())
		dest_nick = "*";
	dest_nick = " " + dest_nick;
	string result = ":localhost" + int_to_string(code) + dest_nick + cmd + msg + endmsg;
	dest.setBuff(dest.getBuff() + result);
	return ;
}

void	Server::flush_all_buffer() {
	for ( map<Socket, Client>::iterator it = this->fd_map.begin(); it != this->fd_map.end(); it++) {
		flush_buff(it->second.getFd());
	}
}

void Server::flush_buff( Socket fd ) {
	string buff;
	buff = this->fd_map[fd].getBuff();
	if (buff.empty())
	{
		return ;
	}
	int ret_val = send(fd, buff.c_str(), buff.length(), 0);
	if ( ret_val == -1 )
	{
		if (errno == EAGAIN)
		{
			return ;
		}
		if ( errno == ECONNRESET )
		{
			this->_disconect_client(fd);
			return ;
		}
		throw invalid_argument(string("send") + strerror(errno));
	}
	if ( ret_val == 0 ) {
		cout << "nothing sent" << endl;
		return ;
	}
	cout << YELLOW << "send " << ret_val << " bytes" << RESET << endl;
	this->fd_map[fd].clearBuff();
}