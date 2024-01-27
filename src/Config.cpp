/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 18:40:27 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/27 16:01:13 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

std::string	Config::_config_content;
std::string	Config::_buffer;

Config::Config() {
}

Config::~Config() {
}

void Config::validateFile(const std::string file_name) {
	std::ifstream is(file_name);
	if (is.is_open())
		is.close();
	else
		throw ReadConfigFileError("Configuration file syntax error: error page [" + file_name + "]: not exist or invalid access rights");
}

void Config::checkRemoveSlash(std::string& path) {
	if (path.at(0) != SLASH_SIGN)
		throw Config::ReadConfigFileError("Configuration file syntax error: invalid path 1");
	if (path.size() > 2 && path.at(path.size() - 1) == SLASH_SIGN)
		path.erase(path.size() - 1, 1);
	if (path.size() > 1 && path.at(path.size() - 1) == SLASH_SIGN)
		throw ReadConfigFileError("Configuration file syntax error: invalid path 2");
	for (std::string::iterator it = path.begin(); it != path.end(); it++)
		if (*it == SLASH_SIGN && *(it + 1) == SLASH_SIGN
			&& (path.compare(HTTP_STR) != 0 || path.compare(HTTPS_STR) != 0))
			throw ReadConfigFileError("Configuration file syntax error: invalid path 3");
}

void Config::checkSpacesNonPrint(const std::string& path) {
	for (size_t i = 0; i < path.size(); i++)
		if (path.at(i) == SPACE_SIGN || std::isprint(path.at(i)) == 0)
			throw ReadConfigFileError("Configuration file syntax error: space or non-printable sign in the path");
}

void Config::isDigitString(const std::string& str,
	const size_t& start, const size_t& finish, const std::string& error_message) {
	for (size_t i = start; i < str.size() && i < finish; i++)
		if (isdigit(str.at(i)) == 0)
			throw ReadConfigFileError(error_message);
}

void Config::splitString(std::vector<std::string>& str_vector, const std::string& str, const char& delim) {
	std::istringstream stream(str);
	std::string token("");
	while (std::getline(stream, token, delim))
		str_vector.push_back(token);
}

void Config::trimSpaceNonPrintBeginEnd(std::string& content) {
	trimSpaceNonPrintBegin(content);
	trimSpaceNonPrintEnd(content);
}

void Config::trimSpaceNonPrintEnd(std::string& content) {
	size_t i = content.size() - 1;
	while (i >= 0 && (content.at(i) == SPACE_SIGN || std::isprint(content.at(i)) == 0))
		i--;
	content.erase(i + 1, content.size() - i);
}

void Config::trimSpaceNonPrintBegin(std::string& content) {
	size_t i = 0;
	while (i < content.size() && (content.at(i) == SPACE_SIGN || std::isprint(content.at(i)) == 0))
		i++;
	content.erase(0, i);
}

void Config::extractDirective(const std::string& content, size_t& start, size_t& finish, const std::string& name) {
	start += name.size();
	finish = content.find_first_of(END_DIRECTIVE_SIGN, start);
	if (finish == std::string::npos)
		throw ReadConfigFileError("Configuration file syntax error: invalid " + name + "directive");
}

void Config::skipSpaceNonPrint(const std::string& content, size_t& i) {
	while (i < content.size() && (content.at(i) == SPACE_SIGN || std::isprint(content.at(i)) == 0))
		i++;
}

void Config::_validateServerBlock(const ServerConfig& server_config) {
	if (server_config.getLocationMap().find(ROOT_LOCATION) == server_config.getLocationMap().end())
		throw ReadConfigFileError("Configuration file syntax error: root location does not defined in the server");
}

void Config::_addServerBlock(std::vector<ServerConfig>& server_config, size_t& start, size_t& finish) {
	ServerConfig config;
	config.getServerBlock() = _config_content.substr(start, finish - start);
	server_config.push_back(config);
	start = finish;
}

void Config::bracesValidation(const std::string& content, size_t& start, size_t& finish) {
	if (start > content.size() - 1 || content.at(start) != BLOCK_OPEN_SIGN)
		throw ReadConfigFileError("Configuration file syntax error: invalid braces");
	start++;
	finish = start;
	size_t braces_not_closed = 1;
	while (finish < content.size() && braces_not_closed) {
		finish++;
		if (content.at(finish) == BLOCK_OPEN_SIGN)
			braces_not_closed++;
		if (content.at(finish) == BLOCK_CLOSE_SIGN)
			braces_not_closed--;
	}
	if (braces_not_closed != 0)
		throw ReadConfigFileError("Configuration file syntax error: invalid braces");
}

void Config::_extractServerBlocks(std::vector<ServerConfig>& config_type) {
	for (size_t start = 0; start < _config_content.size(); start++) {
		try {
			skipSpaceNonPrint(_config_content, start);
			if (start < _config_content.size()
				&& _config_content.compare(start, sizeof(SERVER_BLOCK) - 1, SERVER_BLOCK) == 0) {
				start += sizeof(SERVER_BLOCK) - 1;
				skipSpaceNonPrint(_config_content, start);
				size_t finish = start;
				bracesValidation(_config_content, start, finish);
				_addServerBlock(config_type, start, finish);
			} else {
				if (config_type.size() == 0)
					throw ReadConfigFileError("Configuration file syntax error: no servers defined");
				skipSpaceNonPrint(_config_content, start);
				if (start != _config_content.size())
					throw ReadConfigFileError("Configuration file syntax error: invalid braces");
			}
		} catch (const std::out_of_range& e) {
			break;
		}
	}
}

void Config::_removeComments(std::string& content) {
	for (size_t start = 0; start < content.size(); start++) {
		if (content.at(start) == COMMENT_SIGN) {
			size_t finish = content.find_first_of(NEW_LINE_SIGN, start);
			if (finish == std::string::npos)
				finish = content.size();
			content.erase(start, finish - start);
		}
	}
}

void Config::_readConfigContent(const std::string& config_name) {
	static std::fstream is(config_name);
	if (is.is_open() == false)
		throw ReadConfigFileError("Can not open configuration file [" + config_name + "]");
	while (std::getline(is, _buffer, EOT))
		_config_content += _buffer;
	is.close();
	if (_config_content.empty())
		throw ReadConfigFileError("The configuration file [" + config_name + "] is empty");
	_removeComments(_config_content);
}

void Config::createServerConfig(const std::string& config_name,
	std::vector<ServerConfig>& server_config) {
	_readConfigContent(config_name);
	_extractServerBlocks(server_config);
	for (std::vector<ServerConfig>::iterator it = server_config.begin(); it != server_config.end(); it++) {
		it->parseServerBlock();
		_validateServerBlock(*it);
	}
	_config_content.clear();
	_buffer.clear();
}
