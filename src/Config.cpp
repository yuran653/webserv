/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 18:40:27 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/17 15:00:39 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

std::string	Config::_config_content;
std::string	Config::_buffer;

Config::Config() {
}

Config::~Config() {
}

void Config::_addBlock(std::vector<ServerConfig>& server_config,
	const std::string& content, size_t& start, size_t& finish) {
	ServerConfig config;
	config.server_block = content.substr(start, finish);
	server_config.push_back(config);
}

void Config::_bracesValidation(const std::string& content, size_t& start, size_t& finish) {
	if (content.at(start) != BLOCK_OPEN_SIGN)
		throw ReadConfigFileError("Configuration file syntax error: braces error");
	start++;
	size_t braces_not_closed = 1;
	while (braces_not_closed) {
		finish++;
		try {
			if (content.at(finish) == BLOCK_OPEN_SIGN)
				braces_not_closed++;
			if (content.at(finish) == BLOCK_CLOSE_SIGN)
				braces_not_closed--;
		} catch (const std::out_of_range& e) {
			break;
		};
	}
	if (braces_not_closed != 0)
		throw ReadConfigFileError("Configuration file syntax error: braces error");
}

void Config::_skipSpaceNewLine(const std::string& content, size_t& i) {
	while (content.at(i) == SPACE_SIGN || content.at(i) == NEW_LINE_SIGN)
		i++;
}

template <typename T>
void Config::_extractBlocks(T& config_type, const std::string& content,
	const size_t& name_size, const std::string& name) {
	for (size_t start = 0; start < content.size(); start++) {
		try {
			_skipSpaceNewLine(content, start);
			if (content.compare(start, name_size, name) == 0) {
				start += name_size;
				_skipSpaceNewLine(content, start);
				size_t finish = start;
				_bracesValidation(content, start, finish);
				_addBlock(config_type, content, start, finish);
				start = finish;
			}
			else
				throw ReadConfigFileError("Configuration file syntax error: " + name + "block name error");
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
}
