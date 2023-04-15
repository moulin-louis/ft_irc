/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   restart.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: waxxy <waxxy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/05 14:25:21 by waxxy             #+#    #+#             */
/*   Updated: 2023/04/05 14:25:34 by waxxy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::restart( const vector<string>& params, Client &client)
{
	(void)params;
	if (!client.isOperator)
	{
		add_rply_from_server(":You are not an operator", client, "RESTART", ERR_NOPRIVILEGES);
		return ;
	}
	else
	{
		if (::kill(_pid, SIGUSR1) == -1)
			throw runtime_error(string("restart: kill failed: ") + strerror(errno));
	}
}
