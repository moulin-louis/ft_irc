/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kill.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 16:33:26 by mpignet           #+#    #+#             */
/*   Updated: 2023/04/24 16:22:53 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::kill( const vector<string>& params, Client &client) {
	try {
		if ( params.size() < 2 ) {
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
			vector<string> reason;
			if (params.size() > 2) {
				reason = params;
				reason.erase(reason.begin(), reason.begin() + 1);
				quit(reason, it->second);
				this->_disconect_client(it->first);
			}
			else {
				reason.push_back(":leaving");
				add_cmd_client("leaving", it->second, client, "KILL");
				quit(reason, it->second);
				this->_disconect_client(it->first);
			}
			
			return ;
		}
	}
	add_rply_from_server(" :" + params[0] + " :No such nick/channel", client, "KILL", ERR_NOSUCHNICK);
}