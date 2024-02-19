/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIInterface.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 12:40:47 by jgoldste          #+#    #+#             */
/*   Updated: 2024/02/19 18:36:41 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIINTERFACE_HPP
#define CGIINTERFACE_HPP

#include "Defines.hpp"

#include <cstdlib>
#include <cstring>
#include <ctime>
#include <iostream>
#include <string>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>

class CGIInterface {
	private:
		CGIInterface();
		~CGIInterface();

		static int		_execute(std::string& header, std::string& body_path,
			char**& envp, const std::string& cgi_pass, const int& file_fd);
		static char**	_initArgv(const std::string& cgi_pass);
		static int		_deleteServiceArgs(char**& argv, const int& status);
		static void		_deleteCharArray(char**& array);
		static std::string	_generateFileName(const int& length);
		static size_t	_setBufSize();
		static int		_returnIfExeedsHeaderSize(char**& argv, std::string& header,
			char*& buff, int response_fd, const std::string& body_path);

	public:
		static int	executeCGI(std::string& header, std::string& body_path, char**& envp,
			const std::string& cgi_pass, const std::string& body_temp_path);

		struct CGIInterfaceError : public std::runtime_error {
			CGIInterfaceError(const std::string& message) : std::runtime_error(message) {}
		};
};

#endif
