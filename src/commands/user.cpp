/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   user.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 15:08:23 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/22 16:33:54 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "Server.hpp"

bool is_valid_username(string &username)
{
	if (username.size() > 9)
		return (false);
	//check if username is already taken by another client
	return (true);
}

void	Server::user(vector<string> params, Client& client)
{
	try
	{		
		if (!is_valid_username(params[0]))
			throw invalid_argument("invalid username");
	}
	catch(exception& e)
	{
		//send_client(msg_invalid_user(client), client);
		return ;
	}
	client.username = params[0];
	//send_client(msg_welcome(client), client);
	return ;
}