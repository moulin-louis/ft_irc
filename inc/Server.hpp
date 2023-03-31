/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armendi <armendi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:55:30 by loumouli          #+#    #+#             */
/*   Updated: 2023/03/30 18:27:48 by armendi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#include <string>
#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.hpp"

ssize_t 	sendMessage(Client &, const string& );
void        little_split(vector<string> &list, string &str, const string& delimiter);
string      displayTimestamp();

class Server
{
	private:
        //private function
        Socket			_initiateSocket() ;
        void			_accept_client();
        void			_disconect_client( Socket );
        int				_epoll_ctl_add(int epfd, int fd, uint32_t events);
        //private variable
		const string 	_password;
		const uint16_t	_port;
		Socket			_sfd;
		Epollfd			_epfd;
		epoll_event		_events[MAX_EVENTS];
		string 			_admin_pass;
		string			_motd;
		string			_server_name;
		const string	_server_version;
		const string	_server_up_date;
	public:
		map<Socket, Client>				fd_map;
		map<string, command_function>	cmd_map;
		vector<Channel>					chan_vec;
		vector<string>					ban_word;

        //constructor/destructor
		Server(const char *, const string &);
		~Server();

		//input/output
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
		void	parse_command(string& , Client&  );
		void	nick( vector<string>&, Client& );
		void	pass( vector<string>&, Client& );
		void	ping( vector<string>&, Client& );
		void	user( vector<string>&, Client& );
		void	join( vector<string>&, Client& );
		void	private_msg( vector<string>&, Client& );
		void	oper( vector<string>&, Client& );
		void	quit( vector<string>&, Client& );
		void	mode( vector<string>&, Client& );
		void	topic( vector<string>&, Client& );
		void	kill( vector<string>&, Client& );
		void	part( vector<string>&, Client& );
		void    list( vector<string>&, Client & );
		void	kick( vector<string>&, Client& );
		void    who( vector<string>&, Client & );

		//command utils
		void    process_topic_cmd( vector <string>& , Client& , Channel& );
		void	process_part_cmd( Channel&, Client&, string& );
		void	process_part_cmd( Channel&, Client& );
		void	process_kick_cmd( Channel&, string&, Client&, string& );
        void	handle_without_mask( vector<string>&, Client& );
		void	check_content( vector<string>& );

		//server run functions
		void 	run();

	    //send messages
        void	notify_chan(const string& , const string& , const string& , Client &);
        void	add_cmd_client(const string& , Client& , Client&, const string&  ); //foo
        void	add_cmd_client(const string&, Client&, Client&, const string& , Channel& );

        void displayChannels();
        void getSpecifiedChannels(const vector<string> &params, Client &client);

        //read and setup conf file
        void	read_conf_file();
        void	conf_admin_pass( string& file );
		void	conf_banword_file( string& file );
		void	conf_motd( string& file );
};


#endif