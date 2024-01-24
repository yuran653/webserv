/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 18:41:51 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/24 22:16:27 by jgoldste         ###   ########.fr       */
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
	// std::cout << "NUMBER OF SERVERS: " << server_config.size() << std::endl;
	// for (std::vector<ServerConfig>::iterator it = server_config.begin(); it != server_config.end(); it++) {
	// 	std::cout << "SERVER:"	<< std::endl;
	// 	std::cout << it->getServerBlock() << std::endl;
	// 	std::cout << "->END"	<< std::endl;
	// }
	return 0;
}
