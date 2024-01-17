/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 14:11:04 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/17 18:41:56 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

ServerConfig::ServerConfig() :
	_server_block(),
	_default_server(false),
	_listen(),
	_server_name(),
	_location_map(),
	_error_page() {
}

ServerConfig::ServerConfig(const ServerConfig& other) :
	_server_block(other._server_block),
	_default_server(other._default_server),
	_listen(other._listen),
	_server_name(other._server_name.begin(), other._server_name.end()),
	_location_map(other._location_map.begin(), other._location_map.end()),
	_error_page(other._error_page.begin(), other._error_page.end()) {
}

ServerConfig::~ServerConfig() {
}

ServerConfig& ServerConfig::operator=(const ServerConfig& other) {
	if (this != &other) {
		_server_block = other._server_block;
		_default_server = other._default_server;
		_listen = other._listen;
		_server_name = other._server_name;
		_location_map = other._location_map;
		_error_page = other._error_page;
	}
	return *this;
}

std::string& ServerConfig::getServerBlock() {
	return _server_block;
}

bool ServerConfig::getDefaultServer() {
	return _default_server;
}

std::pair<std::string, int> ServerConfig::getListen() {
	return _listen;
}

std::vector<std::string> ServerConfig::getServerName() {
	return _server_name;
}

std::map<std::string, Location>& ServerConfig::getLocationMap() {
	return _location_map;
}

std::map<int, std::string> ServerConfig::getErrorPage() {
	return _error_page;
}
