/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 18:41:51 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/29 13:02:15 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"
#include "ServerConfig.hpp"
#include "Location.hpp"

#define RED "\033[1;31m"
#define RESET_RED "\033[0m"

int main(int argc, char *argv[]) {
	if (argc > 2) {
		std::cerr << RED "Error:" RESET_RED " Usage: ./webserv or ./webserv [config_file_name].conf" << std::endl;
		return 1;
	}
	std::string config_name(DEFAULT);
	if (argc == 2)
		config_name = std::string(argv[1]);
	static std::vector<ServerConfig> server_config;
	try {
		Config::createServerConfig(config_name, server_config);
	} catch (const Config::ReadConfigFileError& e) {
		std::cerr << RED "Error: " RESET_RED << e.what() << std::endl;
		return 1;
	}
	// ---------------------------------------------------------------- //
	std::cout << "NUMBER OF SERVERS: " << server_config.size() << std::endl;
	// ---------------------------------------------------------------- //
	for (size_t i = 0; i < server_config.size(); i++) {
		std::cout << "SERVER[" << i + 1 << "]:" << std::endl;
		if (server_config.at(i).getServerBlock().empty() == false) {
			std::cerr << "Error: temporary string is not empty" << std::endl;
			return 1;
		}
		if (server_config.at(i).getDefaultServer() == true)
			std::cout << "-> DEFAULT SERVER: ON" << std::endl;
		else
			std::cout << "-> DEFAULT SERVER: OFF" << std::endl;
		std::cout << "-> LISTEN: [" << server_config.at(i).getListen().first << ":"
			<< server_config.at(i).getListen().second << "]" << std::endl;
		std::cout << "-> SERVER NAME: ";
		for (size_t j = 0; j < server_config.at(i).getServerName().size(); j++)
			std::cout << "[" << server_config.at(i).getServerName().at(j) << "]";
		std::cout << std::endl;
		std::map<std::string, Location> location_map = server_config.at(i).getLocationMap();
		for (std::map<std::string, Location>::iterator it = location_map.begin(); it != location_map.end(); it++) {
			std::cout << "-> LOCATION: [" << it->first << "]" << std::endl;
			if (it->second.getAutoindex() == true)
				std::cout << "  -> AUTOINDEX: [ON]" << std::endl;
			else
				std::cout << "  -> AUTOINDEX: OFF" << std::endl;
			std::cout << "  -> ROOT: [" << it->second.getRoot() << "]" << std::endl;
			std::cout << "  -> INDEX: ";
			for (size_t j = 0; j < it->second.getIndex().size(); j++)
				std::cout << "[" << it->second.getIndex().at(j) << "]";
			std::cout << std::endl;
			std::cout << "  -> LIMIT EXCEPT: ";
			for (size_t j = 0; j < it->second.getLimitExcept().size(); j++)
				std::cout << "[" << it->second.getLimitExcept().at(j) << "]";
			std::cout << std::endl;
			std::cout << "  -> RETURN: [" << it->second.getReturn().first << "] -> ["
				<< it->second.getReturn().second << "]" << std::endl;
			// std::cout << "  -> CGI PATH: [" << it->second.getCgiPass() << "]" << std::endl;
			std::cout << "  -> CLIENT BODY: [" << it->second.getClientBodyTempPath() << "]" << std::endl;
			std::cout << "  -> CLIENT MAX SIZE: [" << it->second.getClientMaxBodySize() << "]" << std::endl;
		}
		std::map<int, std::string> error_page = server_config.at(i).getErrorPage(); 
		for (std::map<int, std::string>::iterator it = error_page.begin(); it != error_page.end(); it++)
			std::cout << "-> ERROR CODE: [" << it->first << "] ERROR PATH: [" << it->second << "]" << std::endl;
	}
	// ---------------------------------------------------------------- //
	std::cout << "->OK<-" << std::endl;
	// ---------------------------------------------------------------- //
	return 0;
}
