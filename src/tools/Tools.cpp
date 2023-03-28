//
// Created by tnoulens on 3/22/23.
//

# include "Tools.hpp"

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
			throw std::invalid_argument("Port must be between 1024 and 65535");
		}
	}
	port_nb = std::strtoul(port, NULL, 10);
	if (port_nb > 65535 || port_nb < 1024)
		throw std::invalid_argument("Port must be between 1024 and 65535");
}
