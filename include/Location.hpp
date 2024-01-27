/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 14:07:44 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/27 15:26:43 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <exception>
#include <fstream>
#include <iostream>
#include <sstream>
#include <cmath>
#include <map>
#include <string>
#include <vector>

#include "Defines.hpp"
#include "CodesTypes.hpp"
#include "Config.hpp"

class Location {
	private:
		std::string					_location_block;
		bool						_autoindex;
		std::string					_root;
		std::vector<std::string>	_index;
		std::vector<std::string>	_limit_except;
		std::pair<int, std::string>	_return;
		std::string					_cgi_pass;
		std::string					_client_body_temp_path;
		ssize_t						_client_max_body_size;

	public:
		Location();
		Location(const Location& other);
		~Location();

		Location& operator=(const Location& other);

		std::string&				getLocationBlock();
		const std::string&			getLocationBlock() const;
		bool						getAutoindex() const;
		std::string					getRoot() const;
		std::vector<std::string>	getIndex() const;
		std::vector<std::string>	getLimitExcept() const;
		std::pair<int, std::string>	getReturn() const;
		std::string					getCgiPass() const;
		std::string					getClientBodyTempPath() const;
		size_t						getClientMaxBodySize() const;

		void	parseLocationBlock();
};

#endif
