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

# include "irc.hpp"

class Client
{
	private:
		string		nickname;
		string		username;
		string		realname;
		string		hostname;
		string		buffer;
		Socket		fd;
	public:
        string 	channel;
        Mode	mode[4]; //mode[0] == a ; mode[1] == i ; mode[2] == w ; mode[3] == o
        bool 	passwd_provided;
        bool	 isRegistered;
        bool 	isAway;
        bool 	isOperator;


        //canonical form
		Client(  );
		Client(const Client &);
		~Client();
		Client &operator=(const Client &);

        //getter/setter
		const string 		&getNickname() const;
		const string 		&getUsername() const;
		const string 		&getRealname() const;
		const string 		&getHostname() const;
		const Socket 		&getFd() const;
		string		        getBuff() const;
		Mode				getMode(int );

		void 	setNickname(const string &);
		void 	setUsername(const string &);
		void 	setRealname(const string &);
		void 	setHostname(const string &);
		void 	setFd(const Socket &);
		void 	setBuff( const string&  );
		void 	clearBuff();
		void 	setMode( int , Mode );
};

#endif