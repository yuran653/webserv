/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 18:40:54 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/11 15:18:41 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#define DEFAULT "default.conf"
#define EOT '\x04' // end of transmission

#include "ServerConfig.hpp"
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Config {
	private:
		static std::string					_config_content;
		static std::string					_buffer;

		Config();
		~Config();

		static void	_readConfigContent(const std::string& config_name);
		static void	_parseConfigContent(std::vector<ServerConfig>& server_config);
		static void	_removeComments();

	public:
		static std::vector<ServerConfig>&
			createConfigServer(const std::string& config_name,
				std::vector<ServerConfig>& server_config);

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