/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   irc.hpp                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mpignet <mpignet@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/21 12:12:48 by loumouli          #+#    #+#             */
/*   Updated: 2023/03/24 16:13:38 by mpignet          ###   ########.fr       */
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

#include "colors.h"
#include "Tools.hpp"
#include "cmd_enum.hpp"

typedef int Socket;
typedef int Epollfd;

#define INVALID_SOCKET (-1)
#define SOCKET_ERROR (-1)
#define endmsg string("\r\n")

using namespace std;

#endif