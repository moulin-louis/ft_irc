/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loumouli <loumouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 12:52:07 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/28 20:18:35 by loumouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "Tools.hpp"

string itostr(int i)
{
	stringstream ss;
	ss << i;
	return ss.str();
}

string int_to_string(int i)
{
	if (i == -1) {
		return "";
	}
	stringstream ss;
	if (i < 100) {
		if (i < 10) {
			ss << "00";
		}
		else {
			ss << "0";
		}
	}
	ss << i;
	return ss.str();
}

void	check_port(const char *port)
{
	unsigned long int port_nb;

	for (size_t i = 0; port[i]; i++) {
		if (!isdigit(port[i])) {
			throw invalid_argument("Port must be between 1024 and 65535");
		}
	}
	port_nb = strtoul(port, NULL, 10);
	if (port_nb > 65535 || port_nb < 1024)
		throw invalid_argument("Port must be between 1024 and 65535");
}
