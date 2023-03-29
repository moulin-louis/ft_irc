/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loumouli <loumouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:55:30 by loumouli          #+#    #+#             */
/*   Updated: 2023/03/28 19:56:26 by loumouli         ###   ########.fr       */
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
ssize_t 	sendMessage(Client &client, const std::string& message);
typedef std::map<Socket, Client>::iterator	client_iter;
typedef vector<Channel>::iterator chan_iter;
typedef vector<Socket>::iterator cl_iter;

class Server
{
	typedef	void (Server::*command_function)( vector<string>, Client& );

	private:
		const string 	_password;
		const uint16_t	_port;
		Socket			_sfd;
		Epollfd			_epfd;
		epoll_event		_events[MAX_EVENTS];
		Socket			_initiateSocket() const;
		void			_accept_client();
		void			_disconect_client( Socket );
	public:
		map<Socket, Client>				fd_map;
		map<string, command_function>	cmd_map;
		vector<Channel>					chan_map;

		Server(const char *port, const string &password);
		~Server();

		//request from client
		void	process_input( Socket );

		//respond from server
		void	add_rply_from_server(const string&  msg, Client& dest, const string&  cmd, int code);

		//checks
		Client&		find_user(const string&  nick, Client client);
		Channel&	find_channel(const string&  name, Client client);
		void		is_valid_nickname(string &nickname, Client& client);
		void		is_valid_username(string &username, Client& client);
		void		is_valid_chan_name(vector<string> params, Client& client);

		//commands
		void	parse_command(basic_string<char> input, Client& client );
		void	nick(vector<string> params, Client& client);
		void	pass(vector<string> params, Client& client);
		void	ping(vector<string> params, Client& client);
		void	user(vector<string> params, Client& client);
		void	join(vector<string> params, Client& client);
		void	private_msg(vector<string> params, Client& author);
		void	oper( vector<string> params, Client& client);
		void	quit( vector<string> params, Client& client);
		void	mode( vector<string> params, Client& client );
		void	topic( vector<string> params, Client& client );

		//server run functions
		void 	run();

	//send messages
	void	notify_chan(const string& content, const string& cmd, Client &client);
	void	add_cmd_client(string& content, Client& client, Client& author, const string&  cmd);

};

//string	msg_welcome(Client& client);

#endif