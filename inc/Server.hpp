/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loumouli <loumouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:55:30 by loumouli          #+#    #+#             */
/*   Updated: 2023/03/23 12:30:27 by loumouli         ###   ########.fr       */
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

		void	send_client(string& msg, Client& clt_to);
		void	send_client(string& msg, Client& clt_from, Client& clt_to);
		Client&	find_user(string nick);
		void	is_valid_nickname(string &nickname, Client& client);
		void	is_valid_username(string &username, Client& client);

		//connection commands
		void	nick(vector<string> params, Client& client);
		void	user(vector<string> params, Client& client);
		void	join(vector<string> params, Client& client);
		void	private_msg(vector<string> params, Client& client);
		//Client*	register_connection(string& entry);
		void 	run();
		void	accept_client();
		void	disconect_client( int );
		string	received_data_from_client( Socket );
		void	process_input(Socket);
};

string	msg_welcome(Client& client);

#endif