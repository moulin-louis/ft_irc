/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   private_msg.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:57:21 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/24 16:25:53 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "Server.hpp"

bool	verif_auth(Client& client)
{
	if (client.isRegistered == false)
	{
		string msg = ":localhost " + int_to_string(ERR_NICKNAMEINUSE) + " * * :you are not registered : please set a nickname and a username" + endmsg;
		client.setBuff(client.getBuff() + msg);
		return (false);
	}
	return (true);
}

void	Server::private_msg(vector<string> params, Client& clt_from)
{
	if (params.size() == 0 || params[0].empty())
	{
		throw	invalid_argument("params error");
		return ;
	}
	verif_auth(clt_from);
	try
	{
		Client& dest = find_user(params[0]);
		add_cmd_client(params[1], dest, "PRIVMSG");
	}
	catch (exception& e)
	{
		cout << RED << e.what() << RESET << endl;
		return ;
	}
	return ;
}