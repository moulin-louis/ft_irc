/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:55:30 by loumouli          #+#    #+#             */
/*   Updated: 2023/04/22 16:10:23 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SERVER_HPP
# define SERVER_HPP

# include "irc.hpp"

ssize_t	sendMessage( const Client &, const string& );
void	little_split( vector<string> &, string &, const string& );
string	displayTimestamp();
int		epoll_ctl_add(int , int , uint32_t );
void	add_cmd_client(const string& , Client& , const Client& , const string& );
void	add_cmd_client(const string& , Client& , const Client& , const string&, int );
void	add_cmd_client(const string& , Client& , const Client& , const string& , const Channel& );
void	add_cmd_client(const string& , Client& , const Client& , const string& , const string& , const Channel& );
void	add_cmd_client(const string& , Client& , const Client& , const string& , const Channel& , int );

class Server
{
	private:
        //private function
        Socket			_initiateSocket() ;
        void			_accept_client();
        void			_disconect_client( Socket );
        //private variable
		const pid_t		_pid;
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
		vector<string>	_botList;

	public:
		map<Socket, Client>				fd_map;
		map<string, command_function>	cmd_map;
		vector<Channel>					chan_vec;
		vector<string>					ban_list_server;
        //constructor/destructor
		Server(const char *, const string &);

		~Server();
		//input/output
		void	process_input( Socket );
		void	parse_command(string& , Client&  );
		void 	flush_all_buffers();

		//respond from server
		void	add_rply_from_server(const string& , Client& , const string& , int );
		void    add_rply_from_server(const string &, Client &, Channel &, const string &, int);

		//checks
		Client&		find_user(const string& , Client&, const string&);
		Channel&	find_channel(const string& , Client& );
		void		is_valid_nickname( const string&, Client& );
		void		is_valid_username( const string&, Client& );
		void		is_valid_chan_name( const vector<string>& , Client& );

		//commands
		void	nick( const vector<string>&, Client& );
		void	pass( const vector<string>&, Client& );
		void	ping( const vector<string>&, Client& );
		void	user( const vector<string>&, Client& );
		void	join( const vector<string>&, Client& );
		void	private_msg( const vector<string>&, Client& );
		void	oper( const vector<string>&, Client& );
		void	quit( const vector<string>&, Client& );
		void	mode( const vector<string>&, Client& );
		void	topic( const vector<string>&, Client& );
		void	kill( const vector<string>&, Client& );
		void	part( const vector<string>&, Client& );
		void    list( const vector<string>&, Client & );
		void	kick( const vector<string>&, Client& );
		void    who( const vector<string>&, Client & );
		void	motd( const vector<string>&, Client& );
		void	restart( const vector<string>&, Client& );
		void	wallops( const  vector<string>&, Client& );
		void	names( const vector<string>&, Client& );
		void	away( const vector<string>&, Client& );
		//command utils
		void    process_topic_cmd( const vector <string>& , Client& , Channel& );
		void	process_part_cmd( Channel&, Client&, const string& );
		void	process_part_cmd( Channel&, Client& );
		void	process_kick_cmd( Channel&, const string&, Client& );
		void	process_kick_cmd( Channel&, const string&, Client&, const string& );
		void	handle_user( const vector<string>& params, Client& client, Client &target);
		void    handle_channel(const vector<string> &params, Client &client, Channel &dest);

	//server run functions
		void 	run(const bool& );

	    //send messages
        void	notify_chan(Channel& , const string& , const string& , const Client&);
        void	notify_chan(Channel& , const string& , const string& , const string&, const Client&);
        void	notify_chan(Channel& , const string& , const string& , const Client&, int);

        //read and setup conf file
        void	read_conf_file();
        void	conf_admin_pass( string& );
		void	conf_motd( string& );
		//Getter
		vector<string>&	get_botList();
		//exception
		class NicknameInUse: exception
		{
			public:
				virtual const char* what() const throw();
		};

	void add_rply_from_channel(const string &msg, Client &client, Channel &dest, const string &cmd, int code);
};

#endif