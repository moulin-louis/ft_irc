/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 16:33:26 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/29 14:01:02 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::kill( const vector<string>& params, Client &client) {
	cout << "kill" << endl;
	try {
		if ( params.size() != 2 ) {
			add_rply_from_server(" :Not enough parameters", client, "KILL", ERR_NEEDMOREPARAMS);
			throw invalid_argument("kill: Not enough parameters");
		}
		if (!client.isOperator) {
			add_rply_from_server(":Permission Denied- You're not an IRC operator", client, "KILL", ERR_NOPRIVILEGES);
			throw invalid_argument("kill: Permission Denied- You're not an IRC operator");
		}
	}
	catch (exception& x) {
		cout << RED << x.what() << RESET << endl;
		return ;
	}
	for ( client_iter it = this->fd_map.begin(); it != this->fd_map.end(); ++it) {
		if (it->second.getNickname() == params[0]) {
			add_rply_from_server(string(":") + params[1], client, "KILL", 0);
			sendMessage(client, client.getBuff());
			this->_disconect_client(client.getFd());
			throw runtime_error("client lost connection");
		}
	}
	add_rply_from_server(string(params[0]) + ":No such channel", client, "TOPIC", ERR_NOSUCHCHANNEL);
}