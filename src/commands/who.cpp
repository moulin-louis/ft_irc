/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 11:43:47 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/22 11:45:24 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::who( const vector<string>& params, Client &client) {
	try {
		if (params.empty() ) {
			add_rply_from_server(":Not enough parameters", client, "WHO", ERR_NEEDMOREPARAMS);
			throw invalid_argument("who: Not enough parameters");
		}
		for (chan_iter it = this->chan_vec.begin() ; it != this->chan_vec.end(); ++it ) {
			if (it->getName() == params[0] ) {
				for ( cl_iter x = it->clients.begin(); x != it->clients.end(); ++x ) {
					this->add_rply_from_server(this->fd_map[*x].getNickname(), client, "", RPL_WHOREPLY);
				}
				this->add_rply_from_server(":End of WHO list", client, "", RPL_ENDOFWHO);
			}
		}
	}
	catch ( exception& x) {
		cout << RED << x.what() << RESET << endl;
		return ;
	}
}