/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:55:30 by loumouli          #+#    #+#             */
/*   Updated: 2023/03/27 15:42:16 by mpignet          ###   ########.fr       */
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
		map<int, Client>				fd_map;
		map<string, command_function>	cmd_map;
		vector<Channel>					chan_map;

		Server(const char *port, const string &password);
		Server(const Server &copy);
		~Server();
		Server &operator=(const Server &assign);

		//request from client
		string	received_data_from_client( Socket );
		void	process_input( Socket );

		//respond from server
		void	flush_buff( Socket );
		void	add_cmd_client(string& content, Client& client, Client& author, string cmd);
		void	add_cmd_channel(string& content, Channel& chan, Client& author, string cmd);

		//checks
		Client&		find_user(string nick, Client client);
		Channel&	find_channel(string name, Client client);
		void		is_valid_nickname(string &nickname, Client& client);
		void		is_valid_username(string &username, Client& client);
		void		is_valid_chan_name(vector<string> params);
		void		channel_exists(string &channel_name, Client& client);

		//commands
		void	parse_command( string& input, Client& client );
		void	nick(vector<string> params, Client& client);
		void	pass(vector<string> params, Client& client);
		void	ping(vector<string> params, Client& client);
		void	user(vector<string> params, Client& client);
		void	join(vector<string> params, Client& client);
		void	private_msg(vector<string> params, Client& author);
		void	oper( vector<string> params, Client& client);
		void	quit( vector<string> params, Client& client);

		//server run functions
		void 	run();
		void	accept_client();
		void	disconect_client( Socket );
};

//string	msg_welcome(Client& client);

#endif