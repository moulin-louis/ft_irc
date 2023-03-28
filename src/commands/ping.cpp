/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ping.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:55:30 by loumouli          #+#    #+#             */
/*   Updated: 2023/03/27 17:09:10 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::ping(vector<string> params, Client &client) {
	if ( params.empty() ) {
		add_rply_from_server(":No origin specified", client, "PING", ERR_NOORIGIN);
		return ;
	}
	if ( params[0] != "localhost" && params[0] != "127.0.0.1" ) {
		add_rply_from_server(params[0] + " :No such server", client, "PING", ERR_NOSUCHSERVER);
		return ;
	}
	add_rply_from_server(params[0] + " :Pong", client, "PING", 0);
	return ;
}