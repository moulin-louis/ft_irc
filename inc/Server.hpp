#pragma once
#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <iostream>
# include "irc.hpp"

class Server
{
	private:
		const	string _port;
		const	string _password;
		const	uint16_t		_port_uint;
	public:
		Server(const string &port, const string &password);
		Server(const Server &copy);
		~Server();
		Server &operator=(const Server &assign);

		const string 	&getPort() const;
		uint16_t			getPort_uint() const;
		const string 	&getPassword() const;
};

#endif
