/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 18:40:27 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/16 17:07:02 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

std::string	Config::_config_content;
std::string	Config::_buffer;

Config::Config() {
}

Config::~Config() {
}

// ServerConfig& Config::_addBlock(size_t start, size_t finish) {
// 	ServerConfig server_config;
// 	(void)start;
// 	(void)finish;
// 	return server_config;
// }

void Config::_addBlock(std::vector<ServerConfig>& server_config, size_t start, size_t finish) {
	// ServerConfig config;
	// config._server_block.substr(start, finish);
	// server_config.push_back(config);
	std::cout << "SERVER:"	<< std::endl;
	// std::cout << config._server_block << std::endl;
	while (start < finish) {
		std::cout << _config_content.at(start);
		start++;
	}
	std::cout << "->END"	<< std::endl;
	(void)server_config;
	(void)start;
	(void)finish;
}

void Config::_skipSpaceNewLine(size_t* i) {
	while (_config_content.at(*i) == SPACE_SIGN || _config_content.at(*i) == NEW_LINE_SIGN)
		*i += 1;
}

void Config::_extractBlocks(std::vector<ServerConfig>& server_config) {
	for (size_t config_i = 0; config_i < _config_content.size(); config_i++) {
		try {
			_skipSpaceNewLine(&config_i);
			if (_config_content.compare(config_i, sizeof(SERVER_BLOCK) - 1, SERVER_BLOCK) == 0) {
				config_i += sizeof(SERVER_BLOCK) - 1;
				_skipSpaceNewLine(&config_i);
				if (_config_content.at(config_i) != BLOCK_OPEN_SIGN)
					throw ReadConfigFileError("Configuration file syntax error: braces error");
				config_i++;
				size_t block_i = config_i;
				size_t braces_not_closed = 1;
				while (braces_not_closed) {
					block_i++;
					if (_config_content[block_i] == BLOCK_OPEN_SIGN)
						braces_not_closed++;
					if (_config_content[block_i] == BLOCK_CLOSE_SIGN)
						braces_not_closed--;
				}
				_addBlock(server_config, config_i, block_i);
				config_i = block_i;
			}
			else
				throw ReadConfigFileError("Configuration file syntax error: server block name error");
		} catch (const std::out_of_range& e) {}
	}
	// std::cout << _config_content.at(i) << std::endl;
	(void)server_config;
}

void Config::_removeComments() {
	for (size_t i = 0; i < _config_content.size(); i++) {
		if (_config_content[i] == COMMENT_SIGN) {
			size_t j = i;
			while (_config_content[j] && _config_content[j] != '\n')
				j++;
			_config_content.erase(i, j - i);
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
}

void Config::createServerConfig(const std::string& config_name,
	std::vector<ServerConfig>& server_config) {
	_readConfigContent(config_name);
	_removeComments();
	_extractBlocks(server_config);
}
