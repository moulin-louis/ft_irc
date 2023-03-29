/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   connection.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnoulens <tnoulens@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/28 20:02:16 by loumouli          #+#    #+#             */
/*   Updated: 2023/03/29 15:50:51 by tnoulens         ###   ########.fr       */
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
	this->fd_map.insert(make_pair(csock, temp ));
	char hostname[NI_MAXHOST];
	if (getnameinfo(&csin, sizeof(csin), hostname, sizeof(hostname), NULL, 0, 0) != 0)
		this->fd_map[csock].setHostname("unknown");
	else
		this->fd_map[csock].setHostname(hostname);
	cout << BLUE << "Server: new connection from " << this->fd_map[csock].getHostname() << RESET << endl;
	this->fd_map[csock].setFd(csock);
	epoll_ctl_add(this->_epfd, this->fd_map[csock].getFd(), EPOLLIN | EPOLLOUT | EPOLLHUP | EPOLLRDHUP);
}

void	Server::_disconect_client( Socket fd ) {
	client_iter it = this->fd_map.find(fd);
	epoll_ctl(this->_epfd, EPOLL_CTL_DEL, fd, NULL);
	close(fd);
	if (!it->second.getNickname().empty())
		cout << BLUE << it->second.getNickname() << " closed the connection" << RESET << endl;
	else
		cout << BLUE << it->second.getHostname() << " closed the connection" << RESET << endl;
	this->fd_map.erase(fd);
}