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

#include "irc.hpp"
#include "Server.hpp"

void	Server::pass(vector<string> params, Client& client) {
	if ( params.empty() ) {
		add_rply_from_server(":Not enough parameters", client, "PASS", ERR_NEEDMOREPARAMS);
	}
	if ( client.passwd_provided == true ) {
		add_rply_from_server(":Unauthorized command (already registered)", client, "PASS", ERR_ALREADYREGISTRED);
	}
	if ( params[0] == this->_password ) {
		client.passwd_provided = true;
		return ;
	}
}