#include "Server.hpp"

Server::Server(const std::string &port, const std::string &password): _port(port), _password(password)
{

}

Server::Server(const Server &copy): _port(copy._port), _password(copy._password)
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

const std::string &Server::getPort() const
{
	return (this->_port);
}

const std::string &Server::getPassword() const
{
	return (this->_password);
}
