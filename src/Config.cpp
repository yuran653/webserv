/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 18:40:27 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/03 23:14:43 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

Config::Config() : _config_name("default.conf") {
	_readConfigContent();
}

Config::Config(std::string config_name) : _config_name(config_name) {
	_readConfigContent();
}

Config::Config(const Config& other) :
	_config_name(other._config_name), _config_content(other._config_content) {
}

Config::~Config() {
}

Config& Config::operator=(const Config& other) {
	if (this != &other) {
		const_cast<std::string&>(_config_name) = other._config_name;
		_config_content = other._config_content;
	}
	return *this;
}

void Config::_readConfigContent() {
	std::fstream is(_config_name);
	std::string buffer;
	if (is.is_open() == false)
		throw ReadConfigFileError("Can not open configuration file [" + _config_name + "]");
	while (std::getline(is, buffer, ETX))
		_config_content += buffer;
	is.close();
	if (_config_content.empty())
		throw ReadConfigFileError("The configuration file [" + _config_name + "] is empty");
	std::cout << _config_content << std::endl;
}
