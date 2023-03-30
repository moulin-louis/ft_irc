/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armendi <armendi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:55:30 by loumouli          #+#    #+#             */
/*   Updated: 2023/03/30 16:19:05 by armendi          ###   ########.fr       */
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
typedef map<Socket, Client>::iterator	client_iter;
typedef vector<Channel>::iterator chan_iter;
typedef vector<Socket>::iterator cl_iter;
typedef vector<string>::iterator str_iter;
string	displayTimestamp(void);

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
		string 			admin_pass;
		int				_epoll_ctl_add(int epfd, int fd, uint32_t events);
		string			_server_name;
		const string	_server_version;
		const string	_server_up_date;
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
		Client&		find_user(const string& , Client&, const string&);
		Channel&	find_channel(const string& , Client& );
		void		is_valid_nickname(string &, Client& );
		void		is_valid_username(string &, Client& );
		void		is_valid_chan_name(vector<string> , Client& );

		//commands
		void	parse_command(basic_string<char> , Client&  );
		void	nick(vector<string> , Client& );
		void	pass(vector<string> , Client& );
		void	ping(vector<string> , Client& );
		void	user(vector<string> , Client& );
		void	join(vector<string> , Client& );
		void	private_msg(vector<string> , Client&);
		void	oper( vector<string> , Client& );
		void	quit( vector<string> , Client& );
		void	mode( vector<string> , Client& );
		void	topic( vector<string> , Client& );
		void	kill( vector<string> , Client& );
		void	part( vector<string>, Client& );
		void    list(vector<string> , Client & );
		void	kick(vector<string>, Client&);
		void    who(vector<string> , Client & );

		//command utils
		void    process_topic_cmd(vector <string> , Client& , Channel& );
		void	process_part_cmd(Channel&, Client&, string&);
		void	process_kick_cmd(Channel&, string&, Client&, string&);

		//server run functions
		void 	run();

	//send messages
	void	notify_chan(const string& , const string& , const string& , Client &);
	void	add_cmd_client(const string& , Client& , Client&, const string&  ); //foo
	void	add_cmd_client(const string&, Client&, Client&, const string& , Channel& );


	//who fn
	void	handle_without_mask(vector<string> params, Client& client );
	};

//string	msg_welcome(Client& client);

#endif