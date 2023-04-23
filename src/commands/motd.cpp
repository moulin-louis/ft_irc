/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   motd.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnoulens <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/03 16:13:34 by tnoulens          #+#    #+#             */
/*   Updated: 2023/04/03 16:13:37 by tnoulens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void Server::motd(const vector<string> &params, Client &client)
{
	ifstream file;

	if (params.size() > 1)
	{
		add_rply_from_server(":No such server", client, "", ERR_NOMOTD);
		throw invalid_argument("motd: No such server");
	}
	if (params.size() == 1 && params[0] != this->_server_name)
	{
		add_rply_from_server(":No such server", client, "", ERR_NOMOTD);
		throw invalid_argument("motd: No such server");
	}
	file.open(this->_motd.c_str());
	if (file && file.is_open())
	{
		string line;
		add_rply_from_server(":- " + this->_server_name + " Message of the Day -", client, "", RPL_MOTDSTART);
		while (getline(file, line))
			add_rply_from_server(":- " + line, client, "", RPL_MOTD);
		add_rply_from_server(":End of MOTD command", client, "", RPL_ENDOFMOTD);
		file.close();
	}
	else
	{
		add_rply_from_server(":MOTD File is missing", client, "", ERR_NOMOTD);
		throw invalid_argument("motd: No such server");
	}
}
