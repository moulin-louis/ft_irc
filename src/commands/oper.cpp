/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:55:30 by loumouli          #+#    #+#             */
/*   Updated: 2023/03/27 13:44:32 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::oper(vector<string> params, Client &client) {
	if (params.empty()) {
		string msg = ":localhost " + int_to_string(ERR_NEEDMOREPARAMS) + " " + client.getNickname() + " :Not enough parameters" + endmsg;
		client.setBuff(client.getBuff() + msg);
		return ;
	}
	client.isOperator = true;
	string msg = ":localhost " + int_to_string(RPL_YOUREOPER) + " " + client.getNickname() + " :You are now an IRC operator" + endmsg;
	client.setBuff(client.getBuff() + msg);
	return ;
}