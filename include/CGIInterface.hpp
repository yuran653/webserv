/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIInterface.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 12:40:47 by jgoldste          #+#    #+#             */
/*   Updated: 2024/02/18 20:05:23 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIINTERFACE_HPP
#define CGIINTERFACE_HPP

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

#define MAX_HTTP_HDR 4096
#define BUFF_SIZE 512
#define DBL_CRLF "\r\n\r\n"

class CGIInterface {
	private:
		CGIInterface();
		~CGIInterface();

		static int		_execute(std::string& header, std::string& body_path,
			char**& envp, const std::string& cgi_pass, const int& file_fd);
		static char**	_initArgv(const std::string& cgi_pass);
		static size_t	_setBufSize();
		static int		_deleteServiceArgs(char**& argv, const int& status);
		static void		_deleteCharArray(char**& array);

	public:
		static int	executeCGI(std::string& header, std::string& body_path, char**& envp,
			const std::string& cgi_pass, const std::string& body_temp_path);

		struct CGIInterfaceError : public std::runtime_error {
			CGIInterfaceError(const std::string& message) : std::runtime_error(message) {}
		};
};

#endif
