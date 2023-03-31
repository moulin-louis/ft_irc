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

void	handle_user( Server* server, vector<string>& params, Client& client) {
	if ( params.size() == 1 ) {
		server->add_rply_from_server(mode_to_str(client.mode), client , "MODE", RPL_UMODEIS);
		return ;
	}
	string input = params[1];
	if (input[0] != '+' && input[0] != '-') {
		server->add_rply_from_server(":Please use + or - with mode", client , "MODE", ERR_UMODEUNKNOWNFLAG);
		throw invalid_argument("mode: Please use + or - with mode");
	}
    if (input[0] == '+') {
        for (string::iterator it = input.begin(); it != input.end(); it++ ) {
            if (*it == 'a') {
                server->add_rply_from_server(":Please use AWAY to set your mode to away", client , "MODE", ERR_UMODEUNKNOWNFLAG);
                throw invalid_argument("mode: Please use AWAY to set your mode to away");
            }
            if (*it == 'o' && !client.isOperator) {
                server->add_rply_from_server(":Permission Denied- You're not an IRC operator", client , "MODE", ERR_NOPRIVILEGES);
                throw invalid_argument("mode: Permission Denied- You're not an IRC operator");
            }
            if (*it == 'i') { client.mode[1] = 1; }
            if (*it == 'w') { client.mode[2] = 1; }
        }
    }
    else {
        for (string::iterator it = input.begin(); it != input.end(); it++ ) {
            if (*it == 'a') {
                server->add_rply_from_server(":Please use AWAY to set your mode to away", client , "MODE", ERR_UMODEUNKNOWNFLAG);
                throw invalid_argument("mode: Please use AWAY to set your mode to away");
            }
            if (*it == 'o' && client.isOperator) {
                client.isOperator = false;
                client.mode[3] = 0;
            }
            else {
                throw invalid_argument("mode: Permission Denied- You're not an IRC operator");
            }
            if (*it == 'i') { client.mode[1] = 0; }
            if (*it == 'w') { client.mode[2] = 0; }
        }
    }
	server->add_rply_from_server(mode_to_str(client.mode), client , "MODE", RPL_UMODEIS);
}

void	Server::mode(vector<string>& params, Client &client) {

	try {
		if (params.empty() ) {
			add_rply_from_server(":Not enough parameters", client , "MODE", ERR_NEEDMOREPARAMS);
			throw invalid_argument("mode: Not enough parameters");
		}
		if ( client.getNickname() != params[0] && !client.isOperator) {
			add_rply_from_server(":Cannot change mode for other users", client , "MODE", ERR_USERSDONTMATCH);
			throw invalid_argument("mode: Cannot change mode for other users");
		}
		handle_user(this, params, client);
	}
	catch ( exception& x) {
		cout << RED << x.what() << RESET << endl;
		return ;
	}
}