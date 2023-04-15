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
		if (params.empty()) {
			throw runtime_error("Names: params empyt");
		}
		Channel chan = find_channel(params[0], client);
		cout << "names: chan name is " << chan.getName() << endl;
	}
	catch ( exception& x) {
		cout << RED << x.what() << RESET << endl;
		add_rply_from_server("", client, "", RPL_ENDOFLINKS);
	}
	add_rply_from_server("", client, "", RPL_ENDOFLINKS);
	return ;
}