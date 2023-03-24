/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:55:30 by loumouli          #+#    #+#             */
/*   Updated: 2023/03/24 15:19:40 by mpignet          ###   ########.fr       */
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
# include "Channel.hpp"
#include <functional>

# define MAX_EVENTS 64

class Channel;

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
		map<string, Channel>	chan_map;
		Server(const char *port, const string &password);
		Server(const Server &copy);
		~Server();
		Server &operator=(const Server &assign);

		const uint16_t	&getPort() const;
		const string 	&getPassword() const;
		void	parse_command( string& input, Client& client );

		//send msg functions
		void	add_cmd_client(string& content, Client& client, string cmd);
		//checks
		Client&	find_user(string nick);
		void	is_valid_nickname(string &nickname, Client& client);
		void	is_valid_username(string &username, Client& client);
		void	is_valid_chan_name(vector<string> params);
		void	channel_exists(string &channel_name, Client& client);

		//commands
		void	nick(vector<string> params, Client& client);
		void	pass(vector<string> params, Client& client);
		void	user(vector<string> params, Client& client);
		void	join(vector<string> params, Client& client);
		void	private_msg(vector<string> params, Client& client);

		//server run functions
		void 	run();
		void	accept_client();
		void	disconect_client( Socket );
		string	received_data_from_client( Socket );
		void	process_input( Socket );
		void	flush_buff( Socket );
};

string	msg_welcome(Client& client);

#endif