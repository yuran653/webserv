/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 18:40:54 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/03 23:14:57 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
#define CONFIG_HPP

#define ETX '\x03' // end of text

#include <exception>
#include <fstream>
#include <iostream>
#include <string>

class Config {
	private:
		const std::string	_config_name;
		std::string			_config_content;

		void	_readConfigContent();

	public:
		Config();
		Config(std::string config_name);
		Config(const Config& other);
		~Config();

		Config& operator=(const Config& other);

		struct ReadConfigFileError : public std::runtime_error {
			ReadConfigFileError(const std::string& message) : std::runtime_error(message) {}
		};
};

#endif