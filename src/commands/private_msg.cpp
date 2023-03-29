/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   private_msg.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:57:21 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/27 17:36:39 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "Server.hpp"


void	Server::private_msg(vector<string> params, Client& author) {
	if (params.size() == 0 || params[0].empty()) {
		add_rply_from_server(":No recipient given", author, "PRIVMSG", ERR_NORECIPIENT);
		return ;
	}
	if (params.size() == 1) {
		add_rply_from_server(":No text to send", author, "PRIVMSG", ERR_NOTEXTTOSEND);
		return ;
	}
	try	{
		if (params[0][0] == '#') {
			Channel& dest = find_channel(params[0], author);
			if (dest.user_in_chan(author) == true)
				dest.notify_chan(params[1], "PRIVMSG", author, this);
			else
				add_rply_from_server(":" + dest.getName() + " :Cannot send to channel", author, "PRIVMSG", ERR_CANNOTSENDTOCHAN);
		}
		else {
			Client& dest = find_user(params[0], author);
			this->add_cmd_client(params[1], dest, author, "PRIVMSG");
		}
	}
	catch (exception& e) {
		cout << RED << e.what() << RESET << endl;
		return ;
	}
	return ;
}