

// Assume we have a connected client socket in a variable named "clientSocket"

// Send the MOTD header
send(clientSocket, "375 :- <server> Message of the Day -\r\n", strlen("375 :- <server> Message of the Day -\r\n"), 0);

// Send the MOTD lines
send(clientSocket, "372 :- Welcome to our server! This is the MOTD.\r\n", strlen("372 :- Welcome to our server! This is the MOTD.\r\n"), 0);
send(clientSocket, "372 :- Here is some more information.\r\n", strlen("372 :- Here is some more information.\r\n"), 0);

// Send the end of MOTD marker
send(clientSocket, "376 :End of MOTD command\r\n", strlen("376 :End of MOTD command\r\n"), 0);
