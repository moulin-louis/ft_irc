/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnoulens <tnoulens@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:55:30 by loumouli          #+#    #+#             */
/*   Updated: 2023/03/29 15:50:05 by tnoulens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::quit(vector<string>& params, Client &client) {
	try {
		if ( params.size() != 1 ) {
			throw invalid_argument("quit: Not enough parameters");
		}
		string msg = "ERROR :Closing link: (" + client.getUsername() + "@" + client.getHostname();
		msg += ") [" + params[0] + "]" + endmsg;
		client.setBuff(client.getBuff() + msg);
		sendMessage(client, client.getBuff());
		this->_disconect_client(client.getFd());
		throw runtime_error("client lost connection");
	}
	catch ( runtime_error& x ) {
		throw runtime_error("client lost connection");
	}
	catch ( exception& x ) {
		cout << RED << x.what() << RESET << endl;
	}
}
