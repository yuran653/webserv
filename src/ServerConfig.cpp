/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 14:11:04 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/17 13:02:09 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

ServerConfig::ServerConfig() :
	_default_server(false),
	_listen(),
	_server_name(),
	_location(),
	_error_page(),
	server_block() {
}

ServerConfig::ServerConfig(const ServerConfig& other) :
	_default_server(other._default_server),
	_listen(other._listen),
	_server_name(other._server_name.begin(), other._server_name.end()),
	_location(other._location.begin(), other._location.end()),
	_error_page(other._error_page.begin(), other._error_page.end()),
	server_block(other.server_block) {
}

ServerConfig::~ServerConfig() {
}

ServerConfig& ServerConfig::operator=(const ServerConfig& other) {
	if (this != &other) {
		_default_server = other._default_server;
		_listen = other._listen;
		_server_name = other._server_name;
		_location = other._location;
		_error_page = other._error_page;
		server_block = other.server_block;
	}
	return *this;
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

std::map<std::string, Location> ServerConfig::getLocation() {
	return _location;
}

std::map<int, std::string> ServerConfig::getErrorPage() {
	return _error_page;
}
