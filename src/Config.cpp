/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 18:40:27 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/11 18:54:04 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

std::string	Config::_config_content;
std::string	Config::_buffer;

Config::Config() {
}

Config::~Config() {
}

void Config::_parseConfigContent(std::vector<ServerConfig>& server_config) {
	(void)server_config;
	std::cout << _config_content << std::endl;
}

void Config::_removeComments() {
	for (size_t i = 0; i < _config_content.size(); i++) {
		if (_config_content[i] == '#') {
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

std::vector<ServerConfig>& Config::createConfigServer(const std::string& config_name,
	std::vector<ServerConfig>& server_config) {
	_readConfigContent(config_name);
	_removeComments();
	_parseConfigContent(server_config);
	return server_config;
}


// Config::Config() : _config_name(DEFAULT) {
// 	_createConfigInstance();
// }

// Config::Config(std::string config_name) : _config_name(config_name) {
// 	_createConfigInstance();
// }

// Config::Config(const Config& other) :
// 	_config_name(other._config_name), _config_content(other._config_content) {
// }

// Config::~Config() {
// }

// Config& Config::operator=(const Config& other) {
// 	if (this != &other) {
// 		const_cast<std::string&>(_config_name) = other._config_name;
// 		_config_content = other._config_content;
// 	}
// 	return *this;
// }

// void Config::_parseConfigContent() {
// 	std::cout << "Parsing" << std::endl;
// }

// void Config::_readConfigContent() {
// 	std::fstream is(_config_name);
// 	std::string buffer;
// 	if (is.is_open() == false)
// 		throw ReadConfigFileError("Can not open configuration file [" + _config_name + "]");
// 	while (std::getline(is, buffer, EOT))
// 		_config_content += buffer;
// 	is.close();
// 	if (_config_content.empty())
// 		throw ReadConfigFileError("The configuration file [" + _config_name + "] is empty");
// }

// void Config::_createConfigInstance() {
// 	_readConfigContent();
// 	_parseConfigContent();
// }
