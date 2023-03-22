/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loumouli <loumouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:55:30 by loumouli          #+#    #+#             */
/*   Updated: 2023/03/22 16:32:35 by loumouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef SERVER_HPP
# define SERVER_HPP

# include <string>
# include <iostream>
# include <map>
# include <cstdlib>
# include "irc.hpp"
# include "Client.hpp"

typedef void (*command_function)( vector<string>, Client& );

class Server
{
	private:
		const	string 			_password;
		const	uint16_t		_port;
	public:
		map<int, Client>	fd_map;
		map<string, command_function>	cmd_map;
		Server(const char *port, const string &password);
		Server(const Server &copy);
		~Server();
		Server &operator=(const Server &assign);

		const uint16_t	&getPort() const;
		const string 	&getPassword() const;
		void	parse_command( string& input );

		string	msg_welcome(Client& client);
		string	msg_invalid_nick(Client& client);
		//Client*	register_connection(string& entry);
};

#endif