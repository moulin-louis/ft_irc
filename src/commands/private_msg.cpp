/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   private_msg.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 16:57:21 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/27 15:53:16 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"
#include "Server.hpp"


void	Server::private_msg(vector<string> params, Client& author)
{
	if (params.size() == 0 || params[0].empty()) {
		string msg = ":localhost" + int_to_string(ERR_NORECIPIENT) + " * " + " :No recipient given PRIVMSG" + endmsg;
		author.setBuff(author.getBuff() + msg);
		return ;
	}
	if (params.size() == 1) {
		string msg = ":localhost" + int_to_string(ERR_NOTEXTTOSEND) + " * " + " :No text to send" + endmsg;
		author.setBuff(author.getBuff() + msg);
		return ;
	}
	try	{
		if (params[0][0] == '#') {
			Channel& dest = find_channel(params[0], author);
			if (dest.user_in_chan(author) == true)
				dest.add_cmd_channel(params[1], author, "PRIVMSG");
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