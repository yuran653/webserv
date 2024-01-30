/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 14:07:44 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/30 12:30:34 by jgoldste         ###   ########.fr       */
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
#include <set>
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
		std::set<std::string>		_limit_except; // check fpr temp_path
		std::pair<int, std::string>	_return; // check for root
		std::string					_cgi_pass; // ---?
		std::string					_client_body_temp_path;
		ssize_t						_client_max_body_size;

		void	_assignAutoindex(size_t& start, size_t& finish);
		void	_assignPath(std::string& path, size_t& start, size_t& finish, const std::string& name);
		void	_assignIndex (size_t& start, size_t& finish);
		void	_assignLimitExcept(size_t& start, size_t& finish);
		void	_assignReturn(size_t& start, size_t& finish);
		void	_assignBozySize(size_t& start, size_t& finish);
		void	_caseAutoindex(size_t& start, size_t& finish);
		void	_caseRootReturn(size_t& start, size_t& finish);
		void	_caseIndex(size_t& start, size_t& finish);
		void	_caseLimit(size_t& start, size_t& finish);
		void	_caseCgiTempBody(size_t& start, size_t& finish);

	public:
		Location();
		Location(const Location& other);
		~Location();

		Location& operator=(const Location& other);

		std::string&						getLocationBlock();
		const std::string&					getLocationBlock() const;
		bool								getAutoindex() const;
		const std::string&					getRoot() const;
		const std::vector<std::string>&		getIndex() const;
		const std::set<std::string>&		getLimitExcept() const;
		const std::pair<int, std::string>&	getReturn() const;
		const std::string&					getCgiPass() const;
		const std::string&					getClientBodyTempPath() const;
		size_t								getClientMaxBodySize() const;

		void	parseLocationBlock();
};

#endif
