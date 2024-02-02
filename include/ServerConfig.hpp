/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerConfig.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/08 14:06:01 by jgoldste          #+#    #+#             */
/*   Updated: 2024/02/02 17:22:49 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "Location.hpp"
#include "Config.hpp"

class Location;

class ServerConfig {
	private:
		std::string							_server_block;
		bool								_default_server;
		std::pair<std::string, ssize_t> 	_listen;
		std::vector<std::string>			_server_name;
		std::map<std::string, Location>		_location_map;
		std::map<int, std::string>			_error_page;

		void	_caseListenLocation(size_t& start, size_t& finish);
		void	_caseServerName(size_t& start, size_t& finish);
		void	_caseErrorPage(size_t& start, size_t& finish);
		void	_assignListen(size_t& start, size_t& finish);
		void	_validateHost();
		void	_assignLocation(size_t& start, size_t& finish);
		void	_assignLocationPath(std::string& path, size_t& start, size_t& finish);
		void	_validateWidlcard(const std::string& path);
		void	_addLocationBlock(const std::string& path, size_t& start, size_t& finish);
		void	_assignServerName(size_t& start, size_t& finish);
		void	_assignErrorPage(size_t& start, size_t& finish);
		
	public:
		ServerConfig();
		ServerConfig(const ServerConfig& other);
		~ServerConfig();

		ServerConfig& operator=(const ServerConfig& other);

		std::string&							getServerBlock();
		const std::string&						getServerBlock() const;
		bool									getDefaultServer() const;
		const std::pair<std::string, ssize_t>&		getListen() const;
		const std::vector<std::string>&			getServerName() const;
		const std::map<std::string, Location>&	getLocationMap() const;
		const std::map<int, std::string>&		getErrorPage() const;

		void	parseServerBlock();
};

#endif
