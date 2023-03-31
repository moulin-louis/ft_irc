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

int main( int ac, char **av )
{
	try
	{
		if (ac != 3) throw invalid_argument("Usage: ./ircserv <port> <password>");
		check_port(av[1]);
		Server server(av[1], av[2]);
		server.run();
	}
	catch ( exception& x )
	{
		cout << RED << x.what() << RESET << endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}
