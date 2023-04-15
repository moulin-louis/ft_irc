/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pass.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:55:30 by loumouli          #+#    #+#             */
/*   Updated: 2023/03/27 17:08:12 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::pass( const vector<string>& params, Client& client) {
	try {
		if ( params.empty() ) {
			add_rply_from_server(":Not enough parameters", client, "PASS", ERR_NEEDMOREPARAMS);
			throw invalid_argument("pass: Not enough parameters");
		}
		if ( client.passwd_provided ) {
			add_rply_from_server(":Unauthorized command (already registered)", client, "PASS", ERR_ALREADYREGISTRED);
			throw invalid_argument("pass: Unauthorized command (already registered)");
		}
		if ( params[0] == this->_password ) {
			client.passwd_provided = true;
			return ;
		}
	}
	catch ( exception& x ) {
		cout << RED << x.what() << RESET << endl;
	}
}