/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 14:11:04 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/24 15:25:27 by jgoldste         ###   ########.fr       */
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

void ServerConfig::_assignErrorPage() {
	for (std::map<std::string, Location>::iterator it = _location_map.begin(); it != _location_map.end(); it++)
		std::cout << "Location path -> [" << it->first << "]" << std::endl
		<< "Location block:" << std::endl << it->second.getLocationBlock() << std::endl;
}

void ServerConfig::_assignServerName(size_t& start, size_t& finish) {
	start += sizeof(SERVER_NAME) - 1;
	Config::extractDirective(_server_block, start, finish);
	std::string server_name_directive(_server_block.substr(start, finish - start));
	start = finish;
	Config::trimSpaceBeginEnd(server_name_directive);
	Config::splitString(_server_name, server_name_directive, SPACE_SIGN);
	for (std::vector<std::string>::iterator it = _server_name.begin(); it != _server_name.end(); it++) {
		if (it->compare(DEFAULT_SERVER_SIGN) == 0
			&& (_server_name.size() > 1 || _default_server == false))
			throw Config::ReadConfigFileError("Configuration file syntax error: invalid default server parameter");
	}
}

void ServerConfig::_assignLocationPath(std::string& path, size_t& start, size_t& finish) {
	finish = _server_block.find(BLOCK_OPEN_SIGN);
	if (finish == std::string::npos)
		throw Config::ReadConfigFileError("Configuration file syntax error: invalid braces");
	path = _server_block.substr(start, finish - start);
	Config::trimSpaceBeginEnd(path);
	if (path.empty())
		throw Config::ReadConfigFileError("Configuration file syntax error: invalid location path");
	if (path.at(0) != SLASH_SIGN)
		throw Config::ReadConfigFileError("Configuration file syntax error: invalid location path");
	if (path.size() > 1 && path.at(path.size() - 1) == SLASH_SIGN)
		path.erase(path.size() - 1, 1);
	start = finish;
}

void ServerConfig::_assignLocation(size_t& start, size_t& finish) {
	start += sizeof(LOCATION_BLOCK) - 1;
	finish =  start;
	std::string path("");
	_assignLocationPath(path, start, finish);
	Config::bracesValidation(_server_block, start, finish);
	Location location;
	location.getLocationBlock() = _server_block.substr(start, finish - start);
	start = finish;
	_location_map.insert(std::make_pair(path, location));
}

void ServerConfig::_validateHost() {
	if (_listen.first.compare(LOCAL_HOST_NAME) == 0) {
		_listen.first.clear();
		_listen.first = LOCAL_HOST_ADDR;
	}
	int dot = 0;
	for(size_t start = 0; start < _listen.first.size(); start++) {
		size_t finish = start;
		while (finish < _listen.first.size() && _listen.first.at(finish) != HOST_DELIM) 
			finish++;
		for (size_t i = start; i < _listen.first.size() && i < finish; i++)
			if (isdigit(_listen.first.at(i)) == 0)
				throw Config::ReadConfigFileError("Configuration file syntax error: invalid host parameter");
		size_t host;
		std::istringstream(_listen.first.substr(start, finish - start)) >> host;
		if (host < 0 || host > 255)
			throw Config::ReadConfigFileError("Configuration file syntax error: invalid host parameter");
		if (finish < _listen.first.size() && _listen.first.at(finish) == HOST_DELIM)
			dot++;
		start = finish;
	}
	if (dot != 3)
		throw Config::ReadConfigFileError("Configuration file syntax error: invalid host parameter");
}

void ServerConfig::_assignListen(size_t& start, size_t& finish) {
	start += sizeof(LISTEN) - 1;
	Config::extractDirective(_server_block, start, finish);
	_listen.first = _server_block.substr(start, finish - start);
	Config::trimSpaceBeginEnd(_listen.first);
	if (_listen.first.size() > sizeof(DEFAULT_SERVER)
		&& _listen.first.compare(_listen.first.size() - sizeof(DEFAULT_SERVER) + 1,
		sizeof(DEFAULT_SERVER) - 1, DEFAULT_SERVER) == 0) {
		_default_server = true;
		_listen.first.erase(_listen.first.size() - sizeof(DEFAULT_SERVER) + 1,
			sizeof(DEFAULT_SERVER) - 1);
	}
	size_t port_pos = _listen.first.find(':');
	if (port_pos == std::string::npos)
		throw Config::ReadConfigFileError("Configuration file syntax error: invalid listen directive");
	for (size_t i = port_pos + 1; i < _listen.first.size(); i++)
		if (isdigit(_listen.first.at(i)) == 0)
			throw Config::ReadConfigFileError("Configuration file syntax error: invalid port parameter");
	std::istringstream(_listen.first.substr(port_pos + 1, _listen.first.size() - port_pos)) >> _listen.second;
	_listen.first.erase(port_pos, _listen.first.size() - port_pos);
	_validateHost();
	start = finish;
}

void ServerConfig::parseServerBlock() {
	for (size_t start = 0; start < _server_block.size(); start++) {
		size_t finish = start;
		Config::skipSpaceNewLine(_server_block, start);
		std::cout << "SWITCH CASE: [" << _server_block.at(start) << "]" << std::endl;
		switch (_server_block.at(start)) {
			case LSTN_LOC_LIMIT_SIGN:
				if (_server_block.compare(start, sizeof(LISTEN) - 1, LISTEN) == 0)
					_assignListen(start, finish);
				else if (_server_block.compare(start, sizeof(LOCATION_BLOCK) - 1, LOCATION_BLOCK) == 0)
					_assignLocation(start, finish);
				else
					throw Config::ReadConfigFileError("Configuration file syntax error: invalid server block directive");
				break;
			case SERVER_NAME_SIGN:
				if (_server_block.compare(start, sizeof(SERVER_NAME) - 1, SERVER_NAME) == 0)
					_assignServerName(start, finish);
				else
					throw Config::ReadConfigFileError("Configuration file syntax error: invalid server block directive");
				break;
			case ERROR_PAGE_SIGN:
				if (_server_block.compare(start, sizeof(ERROR_PAGE) - 1, ERROR_PAGE) == 0)
					_assignErrorPage();
				else
					throw Config::ReadConfigFileError("Configuration file syntax error: invalid server block directive");
				break;
			default:
				throw Config::ReadConfigFileError("Configuration file syntax error: invalid directive in server block");
		}
	}
}
