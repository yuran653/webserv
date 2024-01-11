/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 18:41:51 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/11 16:59:24 by jgoldste         ###   ########.fr       */
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
	// std::cout << "Config name in main: " << config_name << std::endl;
	static std::vector<ServerConfig> server_config;
	try {
		Config::createConfigServer(config_name, server_config);
	} catch (const Config::ReadConfigFileError& e) {
		std::cerr << "Error: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}

// int main(int argc, char *argv[]) {
// 	if (argc > 2) {
// 		std::cerr << "Error: Usage: ./webserv or ./webserv [config_file_name].conf" << std::endl;
// 		return 1;
// 	}
// 	std::string config_name(DEFAULT);
// 	if (argc == 2)
// 		config_name = std::string(argv[1]);
// 	try {
// 		Config config(config_name);
// 	} catch (const Config::ReadConfigFileError& e) {
// 		std::cerr << "Error: " << e.what() << std::endl;
// 		return 1;
// 	}
// 	return 0;
// }
