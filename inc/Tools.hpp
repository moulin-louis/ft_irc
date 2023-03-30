//
// Created by tnoulens on 3/22/23.
//
#pragma once
#ifndef TOOLS_HPP
# define TOOLS_HPP

# include <climits>
# include <string>
# include <cstdlib>
# include <iostream>
# include <cstdio>
# include <cerrno>
# include <sstream>
# include "irc.hpp"

using namespace std;

void	check_port(const char *);
string	int_to_string(int );
int		epoll_ctl_add(int , int fd, uint32_t );
string	itostr(int i);
#endif