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
		Client(const Client &);
		~Client();
		Client &operator=(const Client &);

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

		void 	setNickname(const string &);
		void 	setUsername(const string &);
		void 	setRealname(const string &);
		void 	setHostname(const string &);
		void 	setFd(const Socket &);
		void 	setSin(const sockaddr_in &);
		void 	setBuff( const string&  );
		void 	clearBuff();
		void 	setMode( int , Mode );
};

#endif