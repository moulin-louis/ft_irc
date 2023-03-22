#pragma once
#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <iostream>
# include "irc.hpp"

class Server
{
	private:
		const std::string _port;
		const std::string _password;
	public:
		Server(const std::string &port, const std::string &password);
		Server(const Server &copy);
		~Server();
		Server &operator=(const Server &assign);

		const std::string &getPort() const;
		const std::string &getPassword() const;
};

#endif
