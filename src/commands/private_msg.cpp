/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   private_msg.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:57:21 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/22 17:08:03 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "Server.hpp"

bool	verif_auth(Client& clt)
{
	if (clt.getNickname().empty() || clt.getUsername().empty())
	{
		//send_client(msg_not_registered(clt), clt);
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
		send_client(params[1], dest, clt_from);
	}
	catch (exception& e)
	{
		cout << RED << e.what() << RESET << endl;
		return ;
	}
	return ;
}