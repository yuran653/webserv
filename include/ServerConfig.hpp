/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 14:06:01 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/11 19:04:05 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "Location.hpp"
#include <map>

class ServerConfig {
	private:
		std::string						_server_block;
		bool							_default_server;
		std::pair<std::string, int> 	_listen;
		std::vector<std::string>		_server_name;
		std::map<std::string, Location>	_location;
		std::map<int, std::string>		_error_page;

	public:
		ServerConfig();
		ServerConfig(const ServerConfig& other);
		~ServerConfig();

		ServerConfig& operator=(const ServerConfig& other);

		bool							getDefaultServer();
		std::pair<std::string, int>		getListen();
		std::vector<std::string>		getServerName();
		std::map<std::string, Location>	getLocation();
		std::map<int, std::string>		getErrorPage();
};

#endif
