#pragma once
#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <iostream>
# include "irc.hpp"
# include "Client.hpp"

class Server
{
	private:
		const	string		_password;
		const	uint16_t	_port_uint;
//		std::map<int, Client>	_fd_map;
	public:
		Server(const char *port, const string &password);
		Server(const Server &copy);
		~Server();
		Server &operator=(const Server &assign);

		const uint16_t	&getPort() const;
		const string 	&getPassword() const;

	string	msg_accept_connection(Client& client);
	string	msg_refuse_connection(Client& client);
	Client&	register_connection(string& entry);
};

#endif