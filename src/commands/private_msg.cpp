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

#include "Server.hpp"

void	Server::private_msg(vector<string>& params, Client& author) {
	try	{
		if (params.empty() || params[0].empty()) {
			add_rply_from_server(":No recipient given", author, "PRIVMSG", ERR_NORECIPIENT);
			throw invalid_argument("private_msg: No recipient given");
		}
		if (params.size() == 1) {
			add_rply_from_server(":No text to send", author, "PRIVMSG", ERR_NOTEXTTOSEND);
			throw invalid_argument("private_msg: No text to send");
		}
		if (params[0][0] == '#') {
			Channel& dest = find_channel(params[0], author);
			if (dest.user_in_chan(author))
			{				
				string msg = params[1];
                msg.erase(string::size_type(0), 1);
				for (size_t i = 2; i < params.size(); i++) {
					msg += " ";
					msg += params[i];
				}
				this->notify_chan(params[0], msg, "PRIVMSG", author);
			}
			else
				add_rply_from_server(":" + dest.getName() + " :Cannot send to channel", author, "PRIVMSG", ERR_CANNOTSENDTOCHAN);
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
            this->add_cmd_client(msg, dest, author, "PRIVMSG");
		}
	}
	catch ( runtime_error& x) {
		cout << RED << x.what() << RESET << endl;
		add_rply_from_server(":Unauthorized word in your message", author, "PRIVMSG", ERR_CANNOTSENDTOCHAN);
		vector<string> temp;
		temp.push_back("kick for using banword");
		this->quit(temp, author);
	}
	catch (exception& e) {
		cout << RED << e.what() << RESET << endl;
	}
}