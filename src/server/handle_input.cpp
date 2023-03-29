/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 20:03:18 by loumouli          #+#    #+#             */
/*   Updated: 2023/03/29 14:08:23 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::process_input(Socket fd ) {
	cout << "processing input..." << endl;
	client_iter it = this->fd_map.find(fd);
	Client &client = it->second;
	ssize_t byte_count;
	string temp;

	temp.resize(512);
	byte_count = recv(fd, (void *) temp.c_str(), temp.length(), 0);
	if (byte_count == -1) {
		throw runtime_error(string("recv: ") + strerror(errno));
	}
	temp.resize(byte_count);
	cout << YELLOW << byte_count << " bytes received" << endl;
	cout << "string received = " << temp << RESET << endl;
	while (true) {
		cout << "in while input" << endl;
		if (temp.find(endmsg) == string::npos)
			break;
		string tok = temp.substr(0, temp.find(endmsg));
		parse_command(tok, this->fd_map[fd]);
		temp.erase(0, temp.find(endmsg) + 2);
	}
	cout << "trying to send the buff" << endl;
	cout << "size of buff is " << client.getBuff().size() << endl;
	cout << "buff is " << client.getBuff() << endl;
	byte_count = sendMessage(client, client.getBuff());
	if (byte_count == -1) {
		throw runtime_error(string("send: ") + strerror(errno));
	}
	cout << endl << YELLOW << byte_count << " bytes sent" << RESET << endl << endl;
	client.clearBuff();
}

void	Server::parse_command(basic_string<char> input, Client& client ) {
	cout << "Parsing one command" << endl;
	vector<string>	result;
	size_t			pos;
	string delimiter = " ";
	while ((pos = input.find(delimiter)) != string::npos) {
		result.push_back(input.substr(0, pos));
		input.erase(0, pos + delimiter.length());
	}
	result.push_back(input);
	string cmd = result[0];
	result.erase(result.begin());
	map<string, command_function>::iterator it = this->cmd_map.find(cmd);
	if (it == this->cmd_map.end()) {
		return ;
	}
	(this->*(it->second))(result, client);
}