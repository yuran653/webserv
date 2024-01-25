/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 18:41:51 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/25 17:07:40 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

int main(int argc, char *argv[]) {
	if (argc > 2) {
		std::cerr << "Error: Usage: ./webserv or ./webserv [config_file_name].conf" << std::endl;
		return 1;
	}
	std::string config_name(DEFAULT);
	if (argc == 2)
		config_name = std::string(argv[1]);
	static std::vector<ServerConfig> server_config;
	try {
		Config::createServerConfig(config_name, server_config);
	} catch (const Config::ReadConfigFileError& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	//
	std::cout << "NUMBER OF SERVERS: " << server_config.size() << std::endl;
	//
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
		// _location_map
		std::map<int, std::string> error_page = server_config.at(i).getErrorPage(); 
		for (std::map<int, std::string>::iterator it = error_page.begin(); it != error_page.end(); it++)
			std::cout << "-> ERROR CODE: [" << it->first << "] ERROR PATH: [" << it->second << "]" << std::endl;
	}
	std::cout << "WTF" << std::endl;
	return 0;
}
