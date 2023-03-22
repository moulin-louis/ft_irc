/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 12:12:48 by loumouli          #+#    #+#             */
/*   Updated: 2023/03/22 15:36:43 by mpignet          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once
#ifndef IRC_HPP
#define IRC_HPP

#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netdb.h>
#include <unistd.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/epoll.h>
#include <iostream>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <csignal>
#include <cstdio>
#include <algorithm>
#include <vector>

#include "Tools.hpp"

typedef int Socket;

#define INVALID_SOCKET -1
#define SOCKET_ERROR -1

using namespace std;

#endif