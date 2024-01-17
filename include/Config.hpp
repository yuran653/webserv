/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 18:40:54 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/17 14:44:05 by jgoldste         ###   ########.fr       */
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

		static void	_readConfigContent(const std::string& config_name);
		static void	_removeComments(std::string& content);
		template <typename T>
		static void	_extractBlocks(T& config_type, const std::string& content,
			const size_t& name_size, const std::string& name);
		static void	_skipSpaceNewLine(const std::string& content, size_t& i);
		static void	_bracesValidation(const std::string& content, size_t& start, size_t& finish);
		static void	_addBlock(std::vector<ServerConfig>& server_config, const std::string& content,
			size_t& start, size_t& finish);

	public:
		static void createServerConfig(const std::string& config_name,
			std::vector<ServerConfig>& server_config);

		struct ReadConfigFileError : public std::runtime_error {
			ReadConfigFileError(const std::string& message) : std::runtime_error(message) {}
		};
};

#endif
