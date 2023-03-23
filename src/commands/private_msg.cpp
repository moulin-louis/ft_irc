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

void	Server::private_msg(vector<string> params, Client& clt_from)
{
	int	user_fd;
	if (params.size() == 0 || params[0].empty())
	{
		throw	invalid_argument("params error");
		return ;
	}
	user_fd = find_user(params[0]);
	if (user_fd == -1)
		return ;
	(void)clt_from;
	//send_client(params[1], params[0], clt_from);
	return ;
}