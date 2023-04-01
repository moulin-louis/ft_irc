//
// Created by loumouli on 4/1/23.
//

#include "Banbot.hpp"

Banbot::Banbot(  ) {
	this->sfd = 0;
	memset(&(this->sin), 0, sizeof(sin));
	this->sin.sin_family = AF_INET;
}

Banbot::~Banbot() {
	if (this->sfd)
		close(this->sfd);
}
