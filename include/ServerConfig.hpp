/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 14:06:01 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/08 17:53:36 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "Config.hpp"

struct Location {
	bool						_autoindex;
	std::string					_root;
	std::vector<std::string>	_index;
	std::vector<std::string>	_limit_except;
	// std::map<std::string, Location>	_location;
	std::map<int, std::string>	_return;
	std::map<int, std::string>	_error_page;
	std::string					_cgi_pass;
	std::string					_client_body_temp_path;
	size_t						_client_max_body_size;
};

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
		std::string						_client_body_temp_path;
		size_t							_client_max_body_size;
		
	public:
		ServerConfig();
		ServerConfig(const ServerConfig& other);
		~ServerConfig();

		ServerConfig& operator=(const ServerConfig& other);
};

#endif
