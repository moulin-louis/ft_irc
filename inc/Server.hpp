#pragma once
#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <iostream>
# include <map>
# include <cstdlib>
# include "irc.hpp"
# include "Client.hpp"

class Server
{
	private:
		const	string 			_password;
		const	uint16_t		_port;
//		std::map<int, Client>	_fd_map;
	public:
		Server(const char *port, const string &password);
		Server(const Server &copy);
		~Server();
		Server &operator=(const Server &assign);

		const uint16_t	&getPort() const;
		const string 	&getPassword() const;

	void	send_client(string& msg, Client& client);

	//connection commands
	void	nick(vector<string> params, Client& client);
	void	user(vector<string> params, Client& client);
};

#endif