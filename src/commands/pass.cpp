/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:55:30 by loumouli          #+#    #+#             */
/*   Updated: 2023/03/24 15:19:40 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "Server.hpp"

void	Server::pass(vector<string> params, Client& client) {
	if ( params.empty() ) {
		string msg = ":localhost" + int_to_string(ERR_NEEDMOREPARAMS) + " *  * PASS :Not enough parameters" + endmsg;
	}
	if ( client.passwd_provided == true ) {
		string msg = ":localhost" + int_to_string(ERR_ALREADYREGISTRED) + " " + client.getNickname() +" PASS :Not enough parameters" + endmsg;
	}
	if ( params[0] == this->_password ) {
		client.passwd_provided = true;
		return ;
	}
}