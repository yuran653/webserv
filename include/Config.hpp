/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 18:40:54 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/08 16:33:41 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#define DEFAULT "default.conf"
#define EOT '\x04' // end of transmission

#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "ServerConfig.hpp"

class Config {
	private:
		static std::string					_config_content;
		static std::string					_buffer;
		static std::vector<ServerConfig>	_server_config;

		Config();
		~Config();

		static void	_readConfigContent(const std::string& config_name);
		static void	_parseConfigContent();

	public:
		static /*std::vector<ServerConfig>*/void	createConfigServer(const std::string& config_name);

		struct ReadConfigFileError : public std::runtime_error {
			ReadConfigFileError(const std::string& message) : std::runtime_error(message) {}
		};
};

#endif

// class Config {
// 	private:
// 		const std::string	_config_name;
// 		std::string			_config_content;

// 		Config();
// 		~Config();

// 		void	_createConfigInstance();
// 		void	_readConfigContent();
// 		void	_parseConfigContent();

// 	public:
// 		// Config();
// 		// Config(std::string config_name);
// 		// Config(const Config& other);
// 		// ~Config();

// 		Config& operator=(const Config& other);

// 		struct ReadConfigFileError : public std::runtime_error {
// 			ReadConfigFileError(const std::string& message) : std::runtime_error(message) {}
// 		};
// };

// #endif