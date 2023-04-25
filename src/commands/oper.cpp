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

void Server::oper( const vector<string>& params, Client &client) {
	try {
		if (params.empty()) {
			add_rply_from_server(":Not enough parameters", client, "OPER", ERR_NEEDMOREPARAMS);
			throw invalid_argument("oper: Not enough parameters");
		}
		if (params[0] != client.getNickname()) {
			add_rply_from_server(":Username incorrect", client, "OPER", ERR_PASSWDMISMATCH);
			throw invalid_argument("oper: Username incorrect");
		}
		if (params[1] != this->_admin_pass) {
			add_rply_from_server(":Password incorrect", client, "OPER", ERR_PASSWDMISMATCH);
			throw invalid_argument("oper: Password incorrect");
		}
		client.isOperator = true;
		add_rply_from_server(":You are now an IRC operator", client, "", RPL_YOUREOPER);
		vector<string> modeparams;
		modeparams.push_back(client.getNickname());
		modeparams.push_back("+o");
		this->mode(modeparams, client);
	}
	catch ( exception& e ) {
		cout << RED << e.what() << RESET << endl;
	}
}