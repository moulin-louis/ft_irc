/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quit.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnoulens <tnoulens@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 14:55:30 by loumouli          #+#    #+#             */
/*   Updated: 2023/03/29 15:50:05 by tnoulens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::names(vector<string>& params, Client &client ) {
	try {
		Channel chan = find_channel(params[0], client);
		for ( vec_sock_iter it = chan.clients.begin(); it != chan.clients.end(); it++ ) {
			if (!(this->fd_map[*it].getMode() & i)) {
				add_rply_from_server(this->fd_map[*it].getUsername(),  client, "", RPL_LINKS);
			}
		}
	}
	catch ( exception& x){
		cout << RED << x.what() << RESET << endl;
	}
	add_rply_from_server("", client, "", RPL_ENDOFLINKS);
	return ;
}