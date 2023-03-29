/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kick.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 16:24:40 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/29 17:10:12 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::kick(vector<string> params, Client& client)
{
	if (params.size() < 2)
	{
		this->add_rply_from_server(":Not enough parameters", client, "KICK", ERR_NEEDMOREPARAMS);
		return ;
	}
	if (!client.isOperator) {
		add_rply_from_server(":Permission Denied- You're not an IRC operator", client, "KICK", ERR_NOPRIVILEGES);
		return ;
	}
}