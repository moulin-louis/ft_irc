/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   private_msg.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: armendi <armendi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:57:21 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/30 15:56:44 by armendi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "Server.hpp"


void	Server::private_msg(vector<string> params, Client& author) {
	try	{
		if (params.size() == 0 || params[0].empty()) {
			add_rply_from_server(":No recipient given", author, "PRIVMSG", ERR_NORECIPIENT);
			throw invalid_argument("private_msg: No recipient given");
		}
		if (params.size() == 1) {
			add_rply_from_server(":No text to send", author, "PRIVMSG", ERR_NOTEXTTOSEND);
			throw invalid_argument("private_msg: No text to sen");
		}
		if (params[0][0] == '#') {
			Channel& dest = find_channel(params[0], author);
			if (dest.user_in_chan(author))
			{				
				string msg = params[1];
				for (size_t i = 2; i < params.size(); i++) {
					msg += " ";
					msg += params[i];
				}
				this->notify_chan(params[0], msg, "PRIVMSG", author);
			}
			else
				add_rply_from_server(":" + dest.getName() + " :Cannot send to channel", author, "PRIVMSG", ERR_CANNOTSENDTOCHAN);
		}
		else {
			Client& dest = find_user(params[0], author, "PRIVMSG");
			this->add_cmd_client(params[1], dest, author, "PRIVMSG");
		}
	}
	catch (exception& e) {
		cout << RED << e.what() << RESET << endl;
	}
}