/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etetopat <etetopat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 21:27:13 by etetopat          #+#    #+#             */
/*   Updated: 2023/12/14 19:41:50 by etetopat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

/* ----------------------------- Standard C++ ------------------------------- */
#include <iostream>
#include <sstream>  // stringstream
#include <fstream>  // ifstream
#include <string>
#include <cstring>  // memset
#include <cstdlib>  // exit() and EXIT_FAILURE/EXIT_SUCCESS
#include <cctype>   // isspace and isdigit
#include <ctime>    // time_t and struct tm
#include <cstdarg>  // va_list


/* ------------------------------ Standard C -------------------------------- */
#include <fcntl.h>  // fcntl(fd, F_SETFL, O_NONBLOCK, FD_CLOEXEC)
#include <unistd.h> // read/write
#include <dirent.h> // opendir/readdir/closedir


/* ---------------------------- STL Containers ------------------------------ */
#include <algorithm>// find
#include <iterator>
#include <list>
#include <map>
#include <vector>


/* -------------------------------- System ---------------------------------- */
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <signal.h>


/* ------------------------------ Networking -------------------------------- */
#include <arpa/inet.h>  // inet_ntoa
#include <sys/select.h> // select
#include <sys/socket.h> // socket functions
#include <netinet/in.h> // sockaddr_in struct

#define TIMEOUT_SEC 60 // Timeout in seconds before closing a connection if no data has been received

template <typename T>
std::string	toString(T const value) {
	std::stringstream	ss;
	ss << value;
	return ss.str();
}


/* ------------------------------- Utils.c ---------------------------------- */
unsigned int	hexToDec(std::string const& nb);
int				stringToInt(std::string str);

// ...
// ...
// ...

#endif