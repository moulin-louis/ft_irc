/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   private_msg.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:57:21 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/27 14:17:05 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "Server.hpp"

// bool	verif_auth(Client& client)
// {
// 	if (client.isRegistered == false)
// 	{
// 		string msg = ":localhost " + int_to_string(ERR_NICKNAMEINUSE) + " * * :you are not registered : please set a nickname and a username" + endmsg;
// 		client.setBuff(client.getBuff() + msg);
// 		return (false);
// 	}
// 	return (true);
// }

void	Server::private_msg(vector<string> params, Client& client)
{
	//verif_auth(client);
	if (params.size() == 0 || params[0].empty())
	{
		string msg = ":localhost" + int_to_string(ERR_NORECIPIENT) + " * " + " :No recipient given PRIVMSG" + endmsg;
		client.setBuff(client.getBuff() + msg);
		return ;
	}
	if (params.size() == 1)
	{
		string msg = ":localhost" + int_to_string(ERR_NOTEXTTOSEND) + " * " + " :No text to send" + endmsg;
		client.setBuff(client.getBuff() + msg);
		return ;
	}
	try
	{
		// if (params[0][0] == '#')
		// {
		// 	Channel& dest = find_channel(params[0], client);
		// 	add_cmd_channel(params[1], dest, "PRIVMSG");
		// 	return ;
		// }
		Client& dest = find_user(params[0], client);
		add_cmd_client(params[1], dest, "PRIVMSG");
	}
	catch (exception& e)
	{
		cout << RED << e.what() << RESET << endl;
		return ;
	}
	return ;
}