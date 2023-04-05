/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnoulens <tnoulens@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/29 15:50:58 by tnoulens          #+#    #+#             */
/*   Updated: 2023/03/29 15:50:59 by tnoulens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "irc.hpp"

bool server_running = true;
bool server_restarting = true;

void	tmp_handler(int sig, siginfo_t *info, void *context)
{
	(void)info;
	(void)context;
	if (sig == SIGINT || sig == SIGTERM)
	{
		cout << endl << YELLOW << "Signal received" << RESET << endl;
		server_running = false;
		server_restarting = false;
	}
	if (sig == SIGUSR1)
	{
		server_restarting = false;
	}
}

void	signal_handling()
{
	struct sigaction	sa = {};

	sa.sa_flags = SA_RESTART;
	sigemptyset(&sa.sa_mask);
	sa.sa_sigaction = tmp_handler;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGQUIT, &sa, NULL);
	sigaction(SIGTERM, &sa, NULL);
	sigaction(SIGUSR1, &sa, NULL);
}

int main( int ac, char **av )
{
	Server *server = NULL;

	try {
		if (ac != 3) throw invalid_argument("Usage: ./ircserv <port> <password>");
		check_port(av[1]);
		server = new Server(av[1], av[2]);
		signal_handling();
		while (server_running)
		{
			if (!server_restarting)
			{
				delete server;
				cout << YELLOW << "Restarting the server with pass: " << av[2] << " and port: " << av[1] << RESET << endl;
				server = new Server(av[1], av[2]);
				server_restarting = true;
			}
			server->run(server_restarting);
		}
		delete server;
	}
	catch ( exception& x ) {
		cout << RED << x.what() << RESET << endl;
		delete server;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
