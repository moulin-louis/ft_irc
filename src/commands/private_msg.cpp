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

bool	Server::find_user(string nick)
{
	for (map<int, Client>::iterator it = this->fd_map.begin(); it != this->fd_map.end(); it++)
	{
		if (it->second.nickname == nick)
			return true;
	}
	return false;
}

void	Server::private_msg(vector<string> params, Client& client)
{
	if (params.size() == 0 || params[0].empty())
	{
		throw	invalid_argument("params error");
		return ;
	}
	find_user(params[0]);
	(void)client;
	return ;
}