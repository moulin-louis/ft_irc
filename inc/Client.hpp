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
		string 		_awayMsg;
		Socket		fd;
		Mode		modeUser; // Use enum MODES to define flags ex: modeUser = a | i | w | o
	public:
        vector<string> channelsMember;
        bool 	passwd_provided;
        bool	 isRegistered;
        bool 	isAway;
        bool 	isOperator;
		bool 	asTriedNickname;
		bool    isLeaving;
		string		cmd_buff;


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
		Mode				getMode() const;
		const string 		&getAway() const;

		void 	setNickname(const string&);
		void 	setUsername(const string&);
		void 	setRealname(const string&);
		void 	setHostname(const string&);
		void 	setFd(const Socket&);
		void 	setBuff( const string&);
		void 	clearBuff();
		void 	setMode(Mode);
		void	unSetMode(Mode);
		void	setAway(const string &);
};

#endif