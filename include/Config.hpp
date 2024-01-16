/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 18:40:54 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/16 18:51:34 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#define DEFAULT "default.conf"
#define EOT '\x04' // end of transmission
#define COMMENT_SIGN '#'
#define SPACE_SIGN ' '
#define NEW_LINE_SIGN '\n'
#define BLOCK_OPEN_SIGN '{'
#define BLOCK_CLOSE_SIGN '}'
#define SERVER_BLOCK "server "
#define LOCATION_BLOCK "location "

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

		static void			_readConfigContent(const std::string& config_name);
		static void			_removeComments();
		template <typename T>
		static void			_extractBlocks(T& server_config);
		static void			_skipSpaceNewLine(size_t* i);
		static void			_addBlock(std::vector<ServerConfig>& server_config, size_t start, size_t finish);

	public:
		static void createServerConfig(const std::string& config_name,
			std::vector<ServerConfig>& server_config);

		struct ReadConfigFileError : public std::runtime_error {
			ReadConfigFileError(const std::string& message) : std::runtime_error(message) {}
		};
};

#endif
