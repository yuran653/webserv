/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 14:06:01 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/09 19:03:16 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "Config.hpp"

class Location;

class ServerConfig {
	private:
		bool							_default_server;
		bool							_autoindex;
		std::pair<std::string, int> 	_listen;
		std::string						_server_name;
		std::string						_root;
		std::vector<std::string>		_index;
		std::map<std::string, Location>	_location;
		std::map<int, std::string>		_return;
		std::map<int, std::string>		_error_page;

	public:
		ServerConfig();
		ServerConfig(const ServerConfig& other);
		~ServerConfig();

		ServerConfig& operator=(const ServerConfig& other);
};

#endif
