/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 18:40:54 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/16 11:59:10 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#define DEFAULT "default.conf"
#define EOT '\x04' // end of transmission
#define COMMENT_SIGN '#'
#define SPACE_SIGN ' '
#define SERVER_BLOCK "server"
#define LOCATION_BLOCK "location"

#include "ServerConfig.hpp"
#include <exception>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

class Config {
	private:
		static std::string	_config_content;
		static std::string	_buffer;

		Config();
		~Config();

		static void	_readConfigContent(const std::string& config_name);
		static void	_removeComments();
		static void	_extractServers(std::vector<ServerConfig>& server_config);

	public:
		static void createServerConfig(const std::string& config_name,
			std::vector<ServerConfig>& server_config);

		struct ReadConfigFileError : public std::runtime_error {
			ReadConfigFileError(const std::string& message) : std::runtime_error(message) {}
		};
};

#endif
