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
		: _pid(getpid()), _password(password), _port(strtoul(port, NULL, 10)), _server_version("v1.0"),
		  _server_up_date(displayTimestamp()), fd_map() {
	cout << endl << YELLOW << "🅵 🆃" << BLINK_YELLOW << " ->" << YELLOW << "🅸 🆁 🅲" << RESET << endl;
	this->_sfd = _initiateSocket();
	this->_epfd = epoll_create1(EPOLL_CLOEXEC);
	if (this->_epfd == -1) {
		close (this->_sfd);
		throw runtime_error(string("epoll_create1: ") + strerror(errno));
	}
	this->cmd_map.insert(make_pair("JOIN", &Server::join));
	this->cmd_map.insert(make_pair("KICK", &Server::kick));
	this->cmd_map.insert(make_pair("kill", &Server::kill));
	this->cmd_map.insert (make_pair("LIST", &Server::list));
	this->cmd_map.insert(make_pair("MODE", &Server::mode));
	this->cmd_map.insert(make_pair("motd", &Server::motd));
	this->cmd_map.insert(make_pair("NAMES", &Server::names));
	this->cmd_map.insert(make_pair("NICK", &Server::nick));
	this->cmd_map.insert(make_pair("OPER", &Server::oper));
	this->cmd_map.insert(make_pair("PART", &Server::part));
	this->cmd_map.insert(make_pair("PASS", &Server::pass));
	this->cmd_map.insert(make_pair("PING", &Server::ping));
	this->cmd_map.insert(make_pair("PRIVMSG", &Server::private_msg));
	this->cmd_map.insert(make_pair("QUIT", &Server::quit));
	this->cmd_map.insert(make_pair("restart", &Server::restart));
	this->cmd_map.insert(make_pair("TOPIC", &Server::topic));
	this->cmd_map.insert(make_pair("USER", &Server::user));
	this->cmd_map.insert(make_pair("wallops", &Server::wallops));
	this->cmd_map.insert(make_pair("WHO", &Server::who));
	this->cmd_map.insert(make_pair("AWAY", &Server::away));
    this->read_conf_file();
}

void    Server::read_conf_file() {
    fstream conf_file;
    string  file_read;
    conf_file.open("./conf_ircserv.cnf", ios::in);
    if (!conf_file.is_open()) {
        throw runtime_error(string("open conf file:") + strerror(errno));
    }
    while (true) {
		char	temp[10000];
		conf_file.getline(temp, 4294967295);
		file_read += temp;
		if (conf_file.eof()) {
			break ;
		}
		file_read += "\n";
    }
	if (file_read[file_read.size() - 1] != '\n') {
		file_read += "\n";
	}
	conf_admin_pass( file_read );
	conf_motd( file_read );
    conf_file.close();
}

void Server::conf_admin_pass( string &file ) {
	unsigned long tok_pos = file.find("admin_password");
	if (tok_pos == string::npos ) {
		throw runtime_error("open conf file: cant find admin_password key");
	}
	tok_pos += strlen("admin_password=");
	unsigned long nl_pos = file.find('\n', tok_pos);
	if ( nl_pos == string::npos ) {
		throw runtime_error("open conf file: cant find newline for admin_password");
	}
	string temp = file.substr(tok_pos, nl_pos - tok_pos);
	this->_admin_pass = temp;
	cout << BOLD_GREEN <<  "admin pass is [" << this->_admin_pass << "]" << RESET << endl;
}

void	Server::conf_motd( string &file ) {
	//opening the file and reading it into a string
	unsigned long tok_pos = file.find("motd");
	if (tok_pos == string::npos ) {
		throw runtime_error("open conf file: cant find motd key");
	}
	tok_pos += strlen("motd=");
	unsigned long nl_pos = file.find('\n', tok_pos);
	if ( nl_pos == string::npos ) {
		throw runtime_error("open conf file: cant find newline for motd");
	}
	string temp = file.substr(tok_pos, nl_pos - tok_pos);
	this->_motd = temp;
	cout << GREEN << "MOTD file successfully opened " << RESET << endl;
}

Socket	Server::_initiateSocket() {
	sockaddr_in	sin = {};
	int			opt = 1;
	Socket 		sfd;
	char		hostname[NI_MAXHOST];

	sin.sin_addr.s_addr = INADDR_ANY;
	sin.sin_family = AF_INET;
	sin.sin_port = htons(this->_port);
	sfd = socket(AF_INET, SOCK_STREAM, 0);
	if (sfd < 0)
		throw runtime_error(string("socket: ") + strerror(errno));
	cout << GREEN << "Socket created" << RESET << endl;
	if (setsockopt(sfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        close(sfd);
        throw runtime_error(string("setsockopt: ") + strerror(errno));
    }
	if (fcntl(sfd, F_SETFL, O_NONBLOCK) == -1) {
        close(sfd);
        throw runtime_error(string("fcntl: ") + strerror(errno));
    }
	if (bind(sfd, reinterpret_cast<sockaddr *>(&sin), sizeof(sin)) == -1 ) {
        close(sfd);
		throw runtime_error(string("bind: ") + strerror(errno));
    }
	cout << GREEN << "Bind done" << RESET << endl;
	if (listen(sfd, SOMAXCONN) < 0){
        close(sfd);
		throw runtime_error(string("listen: ") + strerror(errno));
    }
	if (getnameinfo(reinterpret_cast<const sockaddr *>(&sin), sizeof(sin), hostname, sizeof(hostname), NULL, 0, 0) != 0) {
		this->_server_name = "unknown";
	}
	else {
		this->_server_name = hostname;
	}
	cout << GREEN << "Socket is listening, server address is: " << this->_server_name << RESET << endl;
	return (sfd);
}
