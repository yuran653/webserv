/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 18:40:27 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/24 14:44:47 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

std::string	Config::_config_content;
std::string	Config::_buffer;

Config::Config() {
}

Config::~Config() {
}

void Config::splitString(std::vector<std::string>& str_vector, std::string& str, char delim) {
	std::istringstream stream(str);
	std::string token("");
	while (std::getline(stream, token, delim))
		str_vector.push_back(token);
}

void Config::trimSpaceBeginEnd(std::string& content) {
	trimSpaceBegin(content);
	trimSpaceEnd(content);
}

void Config::trimSpaceBegin(std::string& content) {
	size_t i = 0;
	while (i < content.size() && (content.at(i) == SPACE_SIGN || content.at(i) == TAB_SIGN))
		i++;
	content.erase(0, i);
}

void Config::trimSpaceEnd(std::string& content) {
	size_t i = content.size() - 1;
	while (i >= 0 && (content.at(i) == SPACE_SIGN || content.at(i) == TAB_SIGN))
		i--;
	content.erase(i + 1, content.size() - i);
}

void Config::extractDirective(std::string& content, size_t& start, size_t& finish) {
	finish = start;
	while (finish < content.size() && content.at(finish) != END_DIRECTIVE_SIGN)
		finish++;
}

void Config::_addBlock(std::map<std::string, Location>& location_map,
	const std::string& content, size_t& start, size_t& finish) {
	Location location;
	location.getLocationBlock() = content.substr(start, finish - start);
	(void)location_map;
}

void Config::_addBlock(std::vector<ServerConfig>& server_config,
	const std::string& content, size_t& start, size_t& finish) {
	ServerConfig config;
	config.getServerBlock() = content.substr(start, finish - start);
	server_config.push_back(config);
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

// void Config::skipSpaceBegin(const std::string& content, size_t& start, size_t& finish) {
// 	while (start < content.size() && start < finish && (content.at(i) == SPACE_SIGN || content.at(i) == TAB_SIGN))
// 		start++;
// }

// void Config::skipSpaceEnd(const std::string& content, size_t& start, size_t& finish) {
// 	while (finish > 0 && finish > start && (content.at(i) == SPACE_SIGN || content.at(i) == TAB_SIGN))
// 		finish--;
// }

void Config::skipSpaceNewLine(const std::string& content, size_t& i) {
	while (i < content.size() && (content.at(i) == NEW_LINE_SIGN
		|| content.at(i) == SPACE_SIGN || content.at(i) == TAB_SIGN))
		i++;
}

template <typename T>
void Config::_extractBlocks(T& config_type, const std::string& content,
	const size_t& name_size, const std::string& name) {
	for (size_t start = 0; start < content.size(); start++) {
		try {
			skipSpaceNewLine(content, start);
			if (start < content.size() && content.compare(start, name_size, name) == 0) {
				start += name_size;
				skipSpaceNewLine(content, start);
				size_t finish = start;
				bracesValidation(content, start, finish);
				_addBlock(config_type, content, start, finish);
				start = finish;
			} else {
				skipSpaceNewLine(content, start);
				if (start != content.size() || config_type.size() == 0)
					throw ReadConfigFileError("Configuration file syntax error");
			}
		} catch (const std::out_of_range& e) {
			break;
		}
	}
}

void Config::_removeComments(std::string& content) {
	for (size_t i = 0; i < content.size(); i++) {
		if (content.at(i) == COMMENT_SIGN) {
			size_t j = i;
			while (content.at(j) && content.at(j) != '\n')
				j++;
			content.erase(i, j - i);
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
	_extractBlocks(server_config, _config_content, sizeof(SERVER_BLOCK) - 1, SERVER_BLOCK);
	for (std::vector<ServerConfig>::iterator it = server_config.begin(); it != server_config.end(); it++)
		it->parseServerBlock();
}
