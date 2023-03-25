/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loumouli <mpignet@student.42.fr>           +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:55:30 by loumouli          #+#    #+#             */
/*   Updated: 2023/03/24 15:19:40 by loumouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::ping(vector<string> params, Client &client) {
	if ( params.empty() ) {
		string msg = ":localhost " + int_to_string(ERR_NOORIGIN) + " " + client.getNickname() + " :No origin specified" + endmsg;
		client.setBuff(client.getBuff() + msg);
		return ;
	}
	if ( params[0] != "localhost" && params[0] != "127.0.0.1" ) {
		string msg = ":localhost " + int_to_string(ERR_NOSUCHSERVER) + " " + client.getNickname() + " " + params[0] + " :No such server" + endmsg;
		client.setBuff(client.getBuff() + msg);
		return ;
	}
	return ;
}