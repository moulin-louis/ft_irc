/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 16:33:26 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/24 16:31:09 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef Client_HPP
# define Client_HPP

# include <string>
# include <iostream>
# include "irc.hpp"

typedef uint32_t Mode;

// create a class for our Server's Client in C++98, based on irssi client type

class Client
{
	private:
		string		nickname;
		string		username;
		string		realname;
		string		hostname;
		string		buffer;
		Socket		fd;
		sockaddr_in	sin;
	public:
		Client( void );
		Client(string &nick, string &user);
		Client(const Client &copy);
		~Client();
		Client &operator=(const Client &assign);

		epoll_event	ev;
		string 	channel;
		Mode	mode[4];
		bool 	passwd_provided;
		bool	 isRegistered;
		bool 	isAway;
		bool 	isOperator;

		const string 		&getNickname() const;
		const string 		&getUsername() const;
		const string 		&getRealname() const;
		const string 		&getHostname() const;
		const Socket 		&getFd() const;
		const sockaddr_in	&getSin() const;
		string		getBuff() const;
		Mode				getMode(int );

		void 	setNickname(const string &nickname);
		void 	setUsername(const string &username);
		void 	setRealname(const string &realname);
		void 	setHostname(const string &hostname);
		void 	setFd(const Socket &fd);
		void 	setSin(const sockaddr_in &sin);
		void 	setBuff( const string& buffer );
		void 	clearBuff();
		void 	setMode( int , Mode );
};

#endif