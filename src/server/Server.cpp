/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: loumouli <loumouli@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 12:52:07 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/28 20:04:50 by loumouli         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

bool stop = false;

void handler(int) {
	cout << endl << YELLOW << "Signal received" << RESET << endl;
	stop = true;
}

Server::~Server() {
	if (this->_sfd >= 0)
		close(this->_sfd);
	if (this->_epfd >= 0)
		close(this->_epfd);
}

void	Server::run() {
	epoll_event ev = {};
	int			nfds;

	if (epoll_ctl_add(this->_epfd, this->_sfd, EPOLLIN) == -1)
		throw runtime_error(string("epoll_ctl: ") + strerror(errno));
	signal(SIGINT, handler);
	while (!stop) {
		nfds = epoll_wait(this->_epfd, this->_events, 10, -1);
		if (nfds == -1) {
			if (errno == EINTR)
				continue;
			throw runtime_error(string("epoll_wait: ") + strerror(errno));
		}
		for (int n = 0; n < nfds; ++n) {
			ev = this->_events[n];
			if (ev.events & EPOLLIN) {
				if (ev.data.fd == this->_sfd) {
					this->_accept_client();
				}
				else {
					try {
						this->process_input(ev.data.fd);
					}
					catch (exception& e) {
						cout << RED << e.what() << RESET << endl;
					}
				}
			}
			if (ev.events & (EPOLLHUP | EPOLLRDHUP)) {

//				this->_disconect_client(ev.data.fd);
			}
		}
	}
	cout << RED << "Server stopped" << RESET << endl;
}