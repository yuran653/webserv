/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 14:11:04 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/09 19:03:46 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerConfig.hpp"

ServerConfig::ServerConfig() {
}

ServerConfig::ServerConfig(const ServerConfig& other) : 
	_default_server(other._default_server),
	_autoindex(other._autoindex),
	_listen(other._listen),
	_server_name(other._server_name),
	_root(other._root),
	_index(other._index.begin(), other._index.end()),
	_location(other._location.begin(), other._location.end()),
	_return(other._return.begin(), other._return.end()),
	_error_page(other._error_page.begin(), other._error_page.end()) {
}

ServerConfig::~ServerConfig() {
}

ServerConfig& ServerConfig::operator=(const ServerConfig& other) {
	if (this != &other) {
		_default_server = other._default_server;
		_autoindex = other._autoindex;
		_listen = other._listen;
		_server_name = other._server_name;
		_root = other._root;
		_index = other._index;
		_location = other._location;
		_return = other._return;
		_error_page = other._error_page;
	}
	return *this;
}
