/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIInterface.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 12:40:47 by jgoldste          #+#    #+#             */
/*   Updated: 2024/02/18 22:24:15 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CGIINTERFACE_HPP
#define CGIINTERFACE_HPP

#include "Deffines.hpp"

#include <cstdlib>
#include <cstring>
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
