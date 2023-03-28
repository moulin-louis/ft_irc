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

void Server::quit(vector<string> params, Client &client) {
	cout << "quit called" << endl;
	if ( params.size() != 1 ) {
		return ;
	}
	string msg = "ERROR :Closing link: (" + client.getUsername() + "@" + client.getHostname();
	msg += ") [" + params[0] + "]" + endmsg;
	client.setBuff(client.getBuff() + msg);
	sendMessage(client, client.getBuff());
//	this->flush_buff(client.getFd());
	this->_disconect_client(client.getFd());
	throw runtime_error("client lost connection");
}
