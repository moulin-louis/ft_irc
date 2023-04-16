//
// Created by loumouli on 4/1/23.
//

#include "Banbot.hpp"
#include <pthread.h>

Banbot::Banbot() {
	memset(&(this->sin), 0, sizeof(sin));
	this->sfd = 0;
	memset(&(this->sin), 0, sizeof(this->sin));
	this->sin.sin_family = AF_INET;
	this->bot_username = "username";
}

Banbot::~Banbot() {
	if (this->sfd)
		close(this->sfd);
}

string  Banbot::getApi()
{
    return (this->_api);
}
