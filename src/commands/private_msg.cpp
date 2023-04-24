/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   private_msg.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/07 15:25:58 by waxxy             #+#    #+#             */
/*   Updated: 2023/04/21 15:19:27 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::private_msg( const vector<string>& params, Client& author) {
	try	{
		if (params.empty() || params[0].empty()) {
			add_rply_from_server(":No recipient given", author, "", ERR_NORECIPIENT);
			throw invalid_argument("private_msg: No recipient given");
		}
		if (params.size() == 1) {
			add_rply_from_server(":No text to send", author, "", ERR_NOTEXTTOSEND);
			throw invalid_argument("private_msg: No text to send");
		}
		if (author.getMode() & r) {
			add_rply_from_server(":You are restricted", author, "", ERR_RESTRICTED);
			throw invalid_argument("private_msg: sender is restricted");
		}
		if (params[0][0] == '#') {
			Channel& dest = find_channel(params[0], author);
			if (dest.user_in_chan(author) || !(dest.getMode() & n))
			{
				string msg = params[1];
                msg.erase(string::size_type(0), 1);
				for (size_t i = 2; i < params.size(); i++) {
					msg += " ";
					msg += params[i];
				}
				this->notify_chan(dest, msg, "PRIVMSG", author);
			}
			else
				add_rply_from_server(":" + dest.getName() + " :Cannot send to channel", author, "", ERR_CANNOTSENDTOCHAN);
		}
		else
        {
            string msg = params[1];
            msg.erase(string::size_type(0), 1);
            for (size_t i = 2; i < params.size(); i++)
            {
                msg += " ";
                msg += params[i];
            }
			Client& dest = find_user(params[0], author, "PRIVMSG");
	        if (dest.getMode() & r) {
		        add_rply_from_server(":User is restricted", author, "", ERR_RESTRICTED);
		        throw invalid_argument("private_msg: receiver is restricted");
	        }
			else if (dest.getMode() & a) {
				add_rply_from_server(dest.getNickname() + ": " + dest.getAway(), author, "", RPL_AWAY);
				return ;
			}
            add_cmd_client(msg, dest, author, "PRIVMSG");
		}
	}
	catch (exception& e) {
		cout << RED << e.what() << RESET << endl;
	}
}
