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

string mode_to_str( const Mode (&arr)[4]) {
	string result = "+";
	if (arr[0]) { result += "a"; }
	if (arr[1]) { result += "i"; }
	if (arr[2]) { result += "w"; }
	if (arr[3]) { result += "o"; }
	return result;
}

void	handle_chan( Server* server, vector<string>& params, Client& client) {
	(void)server;
	(void)params;
	(void)client;
}

void	handle_user( Server* server, vector<string>& params, Client& client) {
	(void)server;
	string input = params[1];
	if ( params.size() == 1 ) {
		string msg = ":localhost " + int_to_string(RPL_UMODEIS) + " " + client.getNickname() + ":";
		msg += mode_to_str(client.mode);
		client.setBuff(client.getBuff() + msg);
		return ;
	}
	if (input[0] != '+' && input[0] != '-') {
		return ;
	}
	for (string::iterator it = input.begin(); it != input.end(); it++ ) {
		if (*it == 'a') {
			string msg = ":localhost " + int_to_string(ERR_UMODEUNKNOWNFLAG) + " ";
			msg += client.getNickname() + " :Please use AWAY to set your mode to away" + endmsg;
			client.setBuff(client.getBuff() + msg);
			return ;
		}
		if (*it == 'o') {
			string msg = ":localhost " + int_to_string(ERR_NOPRIVILEGES) + " ";
			msg += client.getNickname() + " :Please use AWAY to set your mode to away" + endmsg;
			client.setBuff(client.getBuff() + msg);
			return ;
		}
		if (*it == 'i') { client.mode[1] = 1; }
		if (*it == 'w') { client.mode[2] = 1; }
	}
	string msg = ":localhost " + int_to_string(RPL_UMODEIS) + " " + client.getNickname() + ":";
	msg += mode_to_str(client.mode);
	client.setBuff(client.getBuff() + msg);
	return ;
}

void	Server::mode(vector<string> params, Client &client) {
	if (params.empty() ) {
		string msg = ":localhost " + int_to_string(ERR_ALREADYREGISTRED) + " ";
		msg += client.getNickname() + " :Not enough parameters" + endmsg;
		client.setBuff(client.getBuff() + msg);
		return ;
	}
	if ( client.getNickname() != params[0] ) {
		string msg = ":localhost " + int_to_string(ERR_USERSDONTMATCH) + " ";
		msg += client.getNickname() + " :Cannot change mode for other users" + endmsg;
		client.setBuff(client.getBuff() + msg);
		return ;
	}
	if ( this->cmd_map.find(params[0]) != this->cmd_map.end() )
		handle_chan(this, params, client);
	else if (params[0] == client.getNickname())
		handle_user(this, params, client);
	return ;
}