#include "Server.hpp"

Server::Server(const string &port, const string &password): _port(port), _password(password), _port_uint(atoi(port.c_str()))
{

}

Server::Server(const Server &copy): _port(copy._port), _password(copy._password), _port_uint(copy._port_uint)
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

const string &Server::getPort() const
{
	return (this->_port);
}

const string &Server::getPassword() const
{
	return (this->_password);
}

uint16_t Server::getPort_uint() const
{
	return (this->_port_uint);
}
