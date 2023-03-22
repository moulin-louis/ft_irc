//
// Created by tnoulens on 3/22/23.
//

# include "Tools.hpp"

void	check_port(const char *port)
{
	unsigned long port_nb;

	for (size_t i = 0; port[i]; i++)
	{
		if (!isdigit(port[i]))
		{
			throw std::invalid_argument("Port must be between 1024 and 65535");
		}
	}
	port_nb = std::strtoul(port, NULL, 10);
	errno = 0;
	if (port_nb > 65535 || port_nb < 1024 || (port_nb == ULONG_MAX && errno == ERANGE))
		throw std::invalid_argument("Port must be between 1024 and 65535");
}
