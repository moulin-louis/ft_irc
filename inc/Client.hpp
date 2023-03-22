/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loumouli <loumouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 16:33:26 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/22 15:15:22 by loumouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef Client_HPP
# define Client_HPP

# include <string>
# include <iostream>
#include "irc.hpp"

// create a class for our Server's Client in C++98, based on irssi client type

class Client
{
	//private:
	public:
		Client();
		Client(string &nick, string &user);
		Client(const Client &copy);
		~Client();
		Client &operator=(const Client &assign);

		string nickname;
		string username;
		string hostname;
		string realname;

		string channel;

		bool isRegistered;
		bool isAway;
		bool isOperator;
		
		Socket fd;
		sockaddr_in sin;
};

#endif