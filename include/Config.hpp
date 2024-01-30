/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 18:40:54 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/30 16:35:56 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <cstdlib>
#include <cstring>
#include <dirent.h>

#include "Location.hpp"
#include "ServerConfig.hpp"

class Location;
class ServerConfig;

class Config {
	private:
		static std::string	_config_content;
		static std::string	_buffer;

		Config();
		~Config();

		static void	_readConfigContent(const std::string& config_name);
		static void	_removeComments(std::string& content);
		static void	_extractServerBlocks(std::vector<ServerConfig>& config_type);
		static void	_addServerBlock(std::vector<ServerConfig>& server_config,
			size_t& start, size_t& finish);
		static void	_checkDefaultServer(const ServerConfig& server_config);

	public:
		static void createServerConfig(const std::string& config_name,
			std::vector<ServerConfig>& server_config);

		static void	skipSpaceNonPrint(const std::string& content, size_t& i);
		static void	bracesValidation(const std::string& content, size_t& start, size_t& finish);
		static void	extractCodePath(std::string& path,
			int& code, const int& min, const int& max, const std::string& name);
		static void	extractDirective(const std::string& content,
			size_t& start, size_t& finish, const std::string& name);
		static void	trimSpaceNonPrintBegin(std::string& content);
		static void	trimSpaceNonPrintEnd(std::string& content);
		static void	trimSpaceNonPrintBeginEnd(std::string& content);
		static void	splitString(std::vector<std::string>& str_vector, const std::string& str);
		static void	splitString(std::set<std::string>& str_vector, const std::string& str);
		static void	isDigitString(const std::string& str,
			const size_t& start, const size_t& finish, const std::string& error_message);
		static void	extractPath(std::string& path);
		static void	checkSpacesNonPrint(const std::string& path);
		static void	checkRemoveSlash(std::string& path);
		static void validateFile(const std::string& file_name);
		static void	extractValidateCode(const std::string& code_str,
			int& code, const int& min, const int& max);
		static void	validateDirectory(const std::string& path);
		static void	validateFileName(const std::string& file_name);

		struct ReadConfigFileError : public std::runtime_error {
			ReadConfigFileError(const std::string& message) : std::runtime_error(message) {}
		};
};

#endif
