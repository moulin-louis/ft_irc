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
#include <stdexcept>

void	Server::handle_without_mask(vector<string>& params, Client& client ) {
	Channel temp;
	for ( chan_iter it = this->chan_vec.begin(); it != this->chan_vec.end(); it ++ ) {
		if (it->getName() == params[0]) {
			temp = *it;
		}
	}


	for ( client_iter it = this->fd_map.begin(); it != this->fd_map.end(); it++ ) {
		if (it->second.mode[0] && it->second.mode[1]) {
			vec_sock_iter test = find(temp.clients.begin(), temp.clients.end(), it->second.getFd());
			if (test != temp.clients.end())
				add_rply_from_server(it->second.getNickname(), client,"WHO", RPL_WHOREPLY);
		}
	}
	add_rply_from_server(":End of WHO /list", client, "WHO", RPL_ENDOFWHO);
}


//WIP, STILL MISSING A LOT OF THINGS
void Server::who(vector<string>& params, Client &client) {
	try {
		if (params.empty() ) {
			add_rply_from_server(":Not enough parameters", client, "WHO", ERR_NEEDMOREPARAMS);
			throw invalid_argument("who: Not enough parameters");
		}
		if (params.size() == 1) {
			cout << "params size = 1" << endl;
			handle_without_mask(params, client);
		}
	}
	catch ( exception& x) {
		cout << RED << x.what() << RESET << endl;
		return ;
	}
}