/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   oper.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:55:30 by loumouli          #+#    #+#             */
/*   Updated: 2023/03/27 17:07:11 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::oper(vector<string> params, Client &client) {
	if (params.empty()) {
		add_rply_from_server(":Not enough parameters", client, "OPER", ERR_NEEDMOREPARAMS);
		return ;
	}
	client.isOperator = true;
	add_rply_from_server(":You are now an IRC operator", client, "OPER", RPL_YOUREOPER);
	return ;
}