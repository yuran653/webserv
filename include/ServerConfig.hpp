/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 14:06:01 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/18 18:55:38 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "Location.hpp"
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#define LISTEN_DELIM ':'
#define HOST_DELIM '.'
#define DEFAULT_SERVER " default_server"

class ServerConfig {
	private:
		std::string						_server_block;
		bool							_default_server;
		std::pair<std::string, int> 	_listen;
		std::vector<std::string>		_server_name;
		std::map<std::string, Location>	_location_map;
		std::map<int, std::string>		_error_page;

		void	_assignListen(size_t& start, size_t& finish);
		bool	_extractLocation();

	public:
		ServerConfig();
		ServerConfig(const ServerConfig& other);
		~ServerConfig();

		ServerConfig& operator=(const ServerConfig& other);

		std::string&					getServerBlock();
		bool							getDefaultServer();
		std::pair<std::string, int>		getListen();
		std::vector<std::string>		getServerName();
		std::map<std::string, Location>	getLocationMap();
		std::map<int, std::string>		getErrorPage();

		void	parseServerBlock();
};

#endif
