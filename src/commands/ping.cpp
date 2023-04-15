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

void Server::ping( const vector<string>& params, Client &client) {
	try {
		if ( params.empty() ) {
			add_rply_from_server(":No origin specified", client, "PING ", ERR_NOORIGIN);
			throw invalid_argument("ping: No origin specified");
		}
		if ( params[0] != this->_server_name ) {
			add_rply_from_server(params[0] + " :No such server", client, "PING ", ERR_NOSUCHSERVER);
			throw invalid_argument("ping: No such server");
		}
		client.setBuff("PONG " + params[0] + endmsg);
	}
	catch ( exception& x ) {
		cout << RED << x.what() << RESET << endl;
	}
}
