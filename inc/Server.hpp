/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loumouli <loumouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:55:30 by loumouli          #+#    #+#             */
/*   Updated: 2023/03/22 17:56:09 by loumouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <iostream>
# include <map>
# include <cstdlib>
# include <sys/epoll.h>
# include "irc.hpp"
# include "Client.hpp"
#include <functional>

# define MAX_EVENTS 64

typedef void (*command_function)( vector<string>, Client& );

class Server
{
	typedef void (Server::*command_function)( vector<string>, Client& );


	private:
		const string 	_password;
		const uint16_t	_port;
		Socket			_sfd;
		Epollfd			_epfd;
		epoll_event		_events[MAX_EVENTS];
		Socket			_initiateSocket() const;
	public:
		map<int, Client>	fd_map;
		map<string, command_function>	cmd_map;
		Server(const char *port, const string &password);
		Server(const Server &copy);
		~Server();
		Server &operator=(const Server &assign);

		const uint16_t	&getPort() const;
		const string 	&getPassword() const;
		void	parse_command( string& input, Client& client );

		void	send_client(string& msg, Client& client);
		bool	find_user(string nick);

		//connection commands
		void	nick(vector<string> params, Client& client);
		void	user(vector<string> params, Client& client);
		void	join(vector<string> params, Client& client);
		void	private_msg(vector<string> params, Client& client);
		Socket	sfd;
		void	initiateSocket();
		string	msg_welcome(Client& client);
		string	msg_invalid_nick(Client& client);
		//Client*	register_connection(string& entry);
		void run();
};


#endif