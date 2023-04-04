//
// Created by loumouli on 4/1/23.
//

#include "Banbot.hpp"
#include <pthread.h>

Banbot::Banbot() {
	memset(&(this->sin), 0, sizeof(sin));
	this->sin.sin_family = AF_INET;
	pthread_mutex_init(&(this->lock_vec), NULL);
	pthread_mutex_init(&(this->lock_socket), NULL);
	this->bot_username = "username";
}

Banbot::~Banbot() {
	if (this->sfd)
		close(this->sfd);
	pthread_mutex_destroy(&(this->lock_vec));
	pthread_mutex_destroy(&(this->lock_socket));
}
