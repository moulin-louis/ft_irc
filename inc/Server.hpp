/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loumouli <loumouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:55:30 by loumouli          #+#    #+#             */
/*   Updated: 2023/03/29 16:30:50 by loumouli         ###   ########.fr       */
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
ssize_t 	sendMessage(Client &, const string& );
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
		Socket			_initiateSocket() ;
		void			_accept_client();
		void			_disconect_client( Socket );
		string 							admin_pass;
	public:
		map<Socket, Client>				fd_map;
		map<string, command_function>	cmd_map;
		vector<Channel>					chan_vec;

		Server(const char *, const string &);
		~Server();

		//request from client
		void	process_input( Socket );

		//respond from server
		void	add_rply_from_server(const string& , Client& , const string& , int );

		//checks
		Client&		find_user(const string& , Client );
		Channel&	find_channel(const string& , Client );
		void		is_valid_nickname(string &, Client& );
		void		is_valid_username(string &, Client& );
		void		is_valid_chan_name(vector<string> , Client& );

		//commands
		void	parse_command(basic_string<char> input, Client& client );
		void	nick(vector<string> params, Client& client);
		void	pass(vector<string> params, Client& client);
		void	ping(vector<string> params, Client& client);
		void	user(vector<string> params, Client& client);
		void	join(vector<string> params, Client& client);
		void	private_msg(vector<string> params, Client&);
		void	oper( vector<string> params, Client& client);
		void	quit( vector<string> params, Client& client);
		void	mode( vector<string> params, Client& client );
		void	topic( vector<string> params, Client& client );
		void	kill( vector<string> , Client& );
		void	part(vector<string> params, Client& client);
		void	list(vector<string> params, Client &client);

		void    process_topic_cmd(vector <string> , Client& , Channel& );

		//server run functions
		void 	run();

	//send messages
	void	notify_chan(const string& , const string& , const string& , Client &);
	void	add_cmd_client(const string& , Client& , Client&, const string&  ); //foo
	void	add_cmd_client(const string&, Client&, Client&, const string& , Channel& );

};

//string	msg_welcome(Client& client);

#endif