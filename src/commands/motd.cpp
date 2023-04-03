

// Assume we have a connected client socket in a variable named "clientSocket"

// Send the MOTD header
send(clientSocket, "375 :- <server> Message of the Day -\r\n", strlen("375 :- <server> Message of the Day -\r\n"), 0);

// Send the MOTD lines
send(clientSocket, "372 :- Welcome to our server! This is the MOTD.\r\n", strlen("372 :- Welcome to our server! This is the MOTD.\r\n"), 0);
send(clientSocket, "372 :- Here is some more information.\r\n", strlen("372 :- Here is some more information.\r\n"), 0);

// Send the end of MOTD marker
send(clientSocket, "376 :End of MOTD command\r\n", strlen("376 :End of MOTD command\r\n"), 0);


void    cmd_motd(IrcServer *serv, user *currentUser, std::string & args)
{
	(void)args;
	std::fstream    motd_file;
	std::string     line;

	motd_file.open(MOTD_FILE, std::ios::in);
	if (motd_file && motd_file.is_open())
	{
		serv->_tcpServer.add_to_buffer(std::make_pair(currentUser->getSdUser(),
													  send_replies(375, currentUser, serv,
																   serv->_tcpServer.getHostname())));
		while (getline(motd_file, line))
		{
			serv->_tcpServer.add_to_buffer(std::make_pair(currentUser->getSdUser(),
														  send_replies(372, currentUser, serv, line)));
		}
		serv->_tcpServer.add_to_buffer(std::make_pair(currentUser->getSdUser(),
													  send_replies(376, currentUser, serv)));
		motd_file.close();
	}
	else
		serv->_tcpServer.add_to_buffer(std::make_pair(currentUser->getSdUser(),
													  send_replies(422, currentUser, serv)));

}