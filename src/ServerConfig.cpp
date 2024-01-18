/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 14:11:04 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/18 19:05:02 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "ServerConfig.hpp"

ServerConfig::ServerConfig() :
	_server_block(),
	_default_server(false),
	_listen(std::make_pair((""), -1)),
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

std::map<std::string, Location> ServerConfig::getLocationMap() {
	return _location_map;
}

std::map<int, std::string> ServerConfig::getErrorPage() {
	return _error_page;
}

void ServerConfig::_assignListen(size_t& start, size_t& finish) {
	Config::skipSpaceNewLine(_server_block, start);
	start += sizeof(LISTEN) - 1;
	Config::extractDirective(_server_block, start, finish);
	_listen.first = _server_block.substr(start, finish - start);
	
}

void ServerConfig::parseServerBlock() {
	for (size_t start = 0; start < _server_block.size(); start++) {
		size_t finish = start;
		try {
			Config::skipSpaceNewLine(_server_block, start);
			switch (_server_block[start]) {
				case LSTN_LOC_LIMIT_SIGN:
					if (_server_block.compare(start, sizeof(LISTEN) - 1, LISTEN) == 0)
						_assignListen(start, finish);
					else if (_server_block.compare(start, sizeof(LOCATION_BLOCK) - 1, LOCATION_BLOCK) == 0) {
						std::cout << "location" << std::endl;
						while (_server_block.at(start) != BLOCK_OPEN_SIGN)
							start++;
						Config::bracesValidation(_server_block, start, finish);
						start = finish + 1;
					} else
						throw Config::ReadConfigFileError("Configuration file syntax error: invalid server block directive");
					break;
				// case SERVER_NAME_SIGN:
				// 	std::cout << "server_name" << std::endl;
				// 	while (_server_block.at(start) != NEW_LINE_SIGN)
				// 		start++;
				// 	break;
				// case ERROR_PAGE_SIGN:
				// 	std::cout << "error_page" << std::endl;
				// 	while (_server_block.at(start) != NEW_LINE_SIGN)
				// 		start++;
				// 	break;
				// default:
				// 	throw Config::ReadConfigFileError("Configuration file syntax error: invalid directive in server block");
			}
		} catch (const std::out_of_range& e) {
			break;
		}
	}
	// listen
	// server_name
	// location
	// error_page
}
