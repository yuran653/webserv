/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 14:11:04 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/26 22:50:19 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

const std::string& ServerConfig::getServerBlock() const {
	return _server_block;
}

bool ServerConfig::getDefaultServer() const {
	return _default_server;
}

std::pair<std::string, int> ServerConfig::getListen() const {
	return _listen;
}

std::vector<std::string> ServerConfig::getServerName() const {
	return _server_name;
}

const std::map<std::string, Location>& ServerConfig::getLocationMap() const {
	return _location_map;
}

std::map<int, std::string> ServerConfig::getErrorPage() const {
	return _error_page;
}

void ServerConfig::_assignErrorPage(size_t& start, size_t& finish) {
	Config::extractDirective(_server_block, start, finish, ERROR_PAGE);
	std::string error_page_path(_server_block.substr(start, finish - start));
	start = finish;
	size_t error_code_end = error_page_path.find_first_of(SLASH_SIGN);
	if (error_code_end == std::string::npos)
		throw Config::ReadConfigFileError("Configuration file syntax error: invalid error page directive");
	std::string error_code_str(error_page_path.substr(0, error_code_end));
	Config::trimSpaceNonPrintBeginEnd(error_code_str);
	if (error_code_str.size() > 3)
		throw Config::ReadConfigFileError("Configuration file syntax error: invalid error page directive");
	Config::isDigitString(error_code_str, 0, error_code_str.size(),
		"Configuration file syntax error: invalid error page directive");
	int error_code;
	std::istringstream(error_code_str) >> error_code;
	if (CodesTypes::codeMessages.find(error_code) == CodesTypes::codeMessages.end())
		throw Config::ReadConfigFileError("Configuration file syntax error: invalid error page directive");
	error_page_path.erase(0, error_code_end);
	Config::trimSpaceNonPrintBeginEnd(error_page_path);
	Config::checkSpacesNonPrint(error_page_path);
	Config::checkRemoveSlash(error_page_path);
	error_page_path.insert(error_page_path.begin(), DOT);
	if (_error_page.insert(std::make_pair(error_code, error_page_path)).second == false)
		throw Config::ReadConfigFileError("Configuration file syntax error: error page code duplication");
}

void ServerConfig::_assignServerName(size_t& start, size_t& finish) {
	Config::extractDirective(_server_block, start, finish, SERVER_NAME);
	std::string server_name_directive(_server_block.substr(start, finish - start));
	start = finish;
	Config::trimSpaceNonPrintBeginEnd(server_name_directive);
	Config::splitString(_server_name, server_name_directive, SPACE_SIGN);
	for (std::vector<std::string>::iterator it = _server_name.begin(); it != _server_name.end(); it++) {
		if (it->compare(DEFAULT_SERVER_SIGN) == 0) {
			if (_server_name.size() > 1 || _default_server == false)
				throw Config::ReadConfigFileError("Configuration file syntax error: invalid default server parameter");
			it->clear();
		}
	}
}

void ServerConfig::_addLocationBlock(const std::string& path, size_t& start, size_t& finish) {
	Location location;
	location.getLocationBlock() = _server_block.substr(start, finish - start);
	location.parseLocationBlock();
	if (_location_map.insert(std::make_pair(path, location)).second == false)
		throw Config::ReadConfigFileError("Configuration file syntax error: location block name duplication");
	start = finish;
}

void ServerConfig::_assignLocationPath(std::string& path, size_t& start, size_t& finish) {
	start += sizeof(LOCATION_BLOCK) - 1;
	finish = _server_block.find_first_of(BLOCK_OPEN_SIGN, start);
	if (finish == std::string::npos)
		throw Config::ReadConfigFileError("Configuration file syntax error: invalid braces");
	path = _server_block.substr(start, finish - start);
	Config::trimSpaceNonPrintBeginEnd(path);
	Config::checkSpacesNonPrint(path);
	Config::checkRemoveSlash(path);
	start = finish;
	if (path.empty())
		throw Config::ReadConfigFileError("Configuration file syntax error: invalid location path");
}

void ServerConfig::_assignLocation(size_t& start, size_t& finish) {
	std::string path("");
	_assignLocationPath(path, start, finish);
	Config::bracesValidation(_server_block, start, finish);
	_addLocationBlock(path, start, finish);
}

void ServerConfig::_validateHost() {
	if (_listen.first.compare(LOCAL_HOST_NAME) == 0) {
		_listen.first.clear();
		_listen.first = LOCAL_HOST_ADDR;
	}
	int dot = 0;
	for(size_t start = 0; start < _listen.first.size(); start++) {
		size_t finish = _listen.first.find_first_of(HOST_DELIM, start);
		if (finish == std::string::npos) {
			if (dot != 3)
				throw Config::ReadConfigFileError("Configuration file syntax error: invalid host parameter");
			finish = _listen.first.size();
		}
		Config::isDigitString(_listen.first, start, finish,
			"Configuration file syntax error: invalid host parameter");
		ssize_t host;
		std::istringstream(_listen.first.substr(start, finish - start)) >> host;
		start = finish;
		if (host < 0 || host > 255)
			throw Config::ReadConfigFileError("Configuration file syntax error: invalid host parameter");
		if (finish < _listen.first.size() && _listen.first.at(finish) == HOST_DELIM)
			dot++;
	}
}

void ServerConfig::_assignListen(size_t& start, size_t& finish) {
	Config::extractDirective(_server_block, start, finish, LISTEN);
	_listen.first = _server_block.substr(start, finish - start);
	Config::trimSpaceNonPrintBeginEnd(_listen.first);
	if (_listen.first.size() > sizeof(DEFAULT_SERVER)
		&& _listen.first.compare(_listen.first.size() - sizeof(DEFAULT_SERVER) + 1,
		sizeof(DEFAULT_SERVER) - 1, DEFAULT_SERVER) == 0) {
		_default_server = true;
		_listen.first.erase(_listen.first.size() - sizeof(DEFAULT_SERVER) + 1,
			sizeof(DEFAULT_SERVER) - 1);
	}
	size_t port_pos = _listen.first.find_first_of(LISTEN_DELIM);
	if (port_pos == std::string::npos || port_pos == _listen.first.size() - 1)
		throw Config::ReadConfigFileError("Configuration file syntax error: invalid listen directive");
	Config::isDigitString(_listen.first, port_pos + 1, _listen.first.size(),
		"Configuration file syntax error: invalid port parameter");
	std::istringstream(_listen.first.substr(port_pos + 1, _listen.first.size() - port_pos)) >> _listen.second;
	_listen.first.erase(port_pos, _listen.first.size() - port_pos);
	_validateHost();
	start = finish;
}

void ServerConfig::_caseErrorPage(size_t& start, size_t& finish) {
	if (_server_block.compare(start, sizeof(ERROR_PAGE) - 1, ERROR_PAGE) == 0)
		_assignErrorPage(start, finish);
	else
		throw Config::ReadConfigFileError("Configuration file syntax error: invalid server block directive");
}

void ServerConfig::_caseServerName(size_t& start, size_t& finish) {
	if (_server_block.compare(start, sizeof(SERVER_NAME) - 1, SERVER_NAME) == 0)
		_assignServerName(start, finish);
	else
		throw Config::ReadConfigFileError("Configuration file syntax error: invalid server block directive");

}

void ServerConfig::_caseListenLocation(size_t& start, size_t& finish) {
	if (_server_block.compare(start, sizeof(LISTEN) - 1, LISTEN) == 0)
		_assignListen(start, finish);
	else if (_server_block.compare(start, sizeof(LOCATION_BLOCK) - 1, LOCATION_BLOCK) == 0)
		_assignLocation(start, finish);
	else
		throw Config::ReadConfigFileError("Configuration file syntax error: invalid server block directive");

}

void ServerConfig::parseServerBlock() {
	for (size_t start = 0; start < _server_block.size(); start++) {
		Config::skipSpaceNonPrint(_server_block, start);
		if (start >= _server_block.size())
			break;
		size_t finish = start;
			switch (_server_block.at(start)) {
				case LSTN_LOC_LIMIT_SIGN:
					_caseListenLocation(start, finish);
					break;
				case SERVER_NAME_SIGN:
					_caseServerName(start, finish);
					break;
				case ERROR_PAGE_SIGN:
					_caseErrorPage(start, finish);
					break;
				default:
					throw Config::ReadConfigFileError("Configuration file syntax error: invalid directive in server block");
			}
	}
	_server_block.clear();
}
