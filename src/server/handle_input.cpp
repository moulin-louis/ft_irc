/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_input.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnoulens <tnoulens@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 20:03:18 by loumouli          #+#    #+#             */
/*   Updated: 2023/04/04 12:39:58 by loumouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	retryRegister(Client &client, Server &server, string &svrname)
{
	vector<string> retry = vector<string>();
	retry.push_back(client.getNickname());
	retry.push_back(client.getHostname());
	retry.push_back(svrname);
	retry.push_back(":" + client.getNickname());
	server.user(retry, client);
}

void	Server::process_input(Socket fd ) {
	client_iter it = this->fd_map.find(fd);
	Client &client = it->second;
	ssize_t byte_count;
	string temp;

	temp.resize(512);
	byte_count = recv(fd, (void *)(temp.c_str()), temp.length(), MSG_DONTWAIT);
	if (byte_count == -1)
		throw runtime_error(string("recv: ") + strerror(errno));
	if (byte_count == 0) {
		this->_disconect_client(fd);
		return ;
	}
	temp.resize(byte_count);
	cout << YELLOW << "str received = [" << temp << "]" << RESET << endl;
	cout << YELLOW << byte_count << " bytes RECEIVED" << RESET << endl;
	while (true) {
		if (temp.find(endmsg) == string::npos)
			break;
		string tok = temp.substr(0, temp.find(endmsg));
		try {
			parse_command(tok, this->fd_map[fd]);
			temp.erase(0, temp.find(endmsg) + 2);
			if (!client.isRegistered && client.asTriedNickname)
				::retryRegister(client, *this, this->_server_name);
		}
		catch (NicknameInUse& e) {
			cout << RED << "Error: " << e.what() << RESET << endl;
			client.asTriedNickname = true;
			break ;
		}
	}
	cout << YELLOW << "sending = [" << client.getBuff() << "]" << RESET << endl;
	//flushing all buffer from all client
	flush_all_buffers();
}

void	Server::parse_command( string& input, Client& client ) {
	vector<string>	result = vector<string>();
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
	if (!client.isRegistered && cmd != "PASS" && cmd != "NICK" && cmd != "USER" && cmd != "QUIT")
	{
		add_rply_from_server(":You have not registered",client, "", ERR_NOTREGISTERED);
		return ;
	}
	(this->*(it->second))(result, client);
}

void Server::flush_all_buffers() {
	for ( client_iter it = this->fd_map.begin(); it != this->fd_map.end(); ++it ) {
		if (it->second.isLeaving)
			continue ;
        cout << "flushing buffer of " << it->second.getNickname() << endl;
        ssize_t byte_count = sendMessage(it->second, it->second.getBuff());
        if (byte_count == -1)
        {
            throw runtime_error(string("send: ") + strerror(errno));
        }
        cout << YELLOW << byte_count << " bytes SENT" << RESET << endl << endl;
		it->second.clearBuff();
	}
}
