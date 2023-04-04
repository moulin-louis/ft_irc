/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ban.cpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: waxxy <waxxy@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/31 14:11:00 by waxxy             #+#    #+#             */
/*   Updated: 2023/03/31 14:11:01 by waxxy            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::ban(vector<string> &params, Client &client)
{
	(void) params;
	(void) client;
	if (params.size() != 2)
		throw invalid_argument("ban: invalid number of parameters");
	if (!client.isOperator)
		throw invalid_argument("ban: you are not an operator");
	if (params[0][0] != '#')
		throw invalid_argument("ban: invalid channel name");
//	if (params[1][0] != ':')
//		throw invalid_argument("ban: invalid ban mask");
	for (chan_iter it = this->chan_vec.begin(); it != this->chan_vec.end(); it++)
		if (it->getName() == params[0])
		{
			it->ban.push_back(params[1].substr(1));
			return;
		}
	throw invalid_argument("ban: channel not found");
}