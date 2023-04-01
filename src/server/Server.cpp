/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tnoulens <tnoulens@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 12:52:07 by mpignet           #+#    #+#             */
/*   Updated: 2023/03/29 15:50:28 by tnoulens         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"
#include "cmd_enum.hpp"
#include "typedef.hpp"

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
	for( client_iter it = this->fd_map.begin(); it != this->fd_map.end(); ) {
		this->add_rply_from_server(":Server is stoping", it->second, "ERROR", ERR_RESTRICTED);
		sendMessage(it->second, it->second.getBuff());
		it++;
	}
}

void Server::run() {
	epoll_event ev = {};
	int nfds;

	if (this->_epoll_ctl_add(this->_epfd, this->_sfd, EPOLLIN) == -1)
		throw runtime_error(string("epoll_ctl: ") + strerror(errno));
	signal(SIGINT, handler);
	while (!stop)
	{
		nfds = epoll_wait(this->_epfd, this->_events, 10, -1);
		if (nfds == -1) {
			if (errno == EINTR)
				continue;
			throw runtime_error(string("epoll_wait: ") + strerror(errno));
		}
		for (int n = 0; n < nfds; ++n) {
			ev = this->_events[n];
			if (ev.events & EPOLLIN) {
				if (ev.data.fd == this->_sfd)
					this->_accept_client();
				else {
					try {
						this->process_input(ev.data.fd);
					}
					catch (exception& e) {
						cout << RED << e.what() << RESET << endl;
					}
				}
			}
		}
	}
	cout << RED << "Stoping	 the server..." << RESET << endl;
}
