#include "Server.hpp"

Server::Server(const char *port, const string &password): _password(password), _port(std::strtoul(port, NULL, 10))//, _fd_map()
{
}

Server::Server(const Server &copy): _password(copy._password), _port(copy._port)//, _fd_map(copy._fd_map)
{
	*this = copy;
}

Server::~Server()
{

}

Server &Server::operator=(const Server &assign)
{
	(void) assign;
	return (*this);
}

const string &Server::getPassword() const
{
	return (this->_password);
}

const uint16_t &Server::getPort() const
{
	return (this->_port);
}
