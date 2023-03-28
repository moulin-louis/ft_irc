/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loumouli <loumouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 20:02:16 by loumouli          #+#    #+#             */
/*   Updated: 2023/03/28 20:02:29 by loumouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

void	Server::_accept_client( void ) {
	sockaddr csin = {};
	socklen_t crecsize = sizeof(csin);
	Client	temp;

	Socket csock = accept(this->_sfd, (struct sockaddr *)&csin, &crecsize);
	if (csock < 0)
		throw runtime_error(string("accept: ") + strerror(errno));
	cout << GREEN << "New connection" << RESET << endl;
	this->fd_map.insert(make_pair(csock, Client()) );
	char hostname[NI_MAXHOST];
	if (getnameinfo(&csin, sizeof(csin), hostname, sizeof(hostname), NULL, 0, 0) != 0)
		this->fd_map[csock].setHostname("unknown");
	else
		this->fd_map[csock].setHostname(hostname);
	std::cout << "Server: got connection from " << this->fd_map[csock].getHostname() << std::endl;
	this->fd_map[csock].setFd(csock);
	epoll_ctl_add(this->_epfd, this->fd_map[csock].getFd(), EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLRDHUP);
}

void	Server::_disconect_client( Socket fd ) {
	client_iter it = this->fd_map.find(fd);
	epoll_ctl(this->_epfd, EPOLL_CTL_DEL, fd, NULL);
	close(fd);
	if (!it->second.getNickname().empty())
		std::cout << GREEN << it->second.getNickname() << " closed the connection" << RESET << std::endl;
	else
		std::cout << GREEN << it->second.getHostname() << " closed the connection" << RESET << std::endl;
	if ( this->fd_map.erase(fd) == 0 )
		cout << RED << "problem deleting client from database" << RESET << endl;
}