/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 14:07:44 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/10 15:37:18 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include <string>
#include <vector>

class Location {
	private:
		bool						_autoindex;
		std::string					_root;
		std::vector<std::string>	_index;
		std::vector<std::string>	_limit_except;
		std::pair<int, std::string>	_return;
		std::string					_cgi_pass;
		std::string					_client_body_temp_path;
		size_t						_client_max_body_size;


	public:
		Location();
		Location(const Location& other);
		~Location();

		Location& operator=(const Location& other);

		bool						getAutoindex();
		std::string					getRoot();
		std::vector<std::string>	getIndex();
		std::vector<std::string>	getLimitExcept();
		std::pair<int, std::string>	getReturn();
		std::string					getCgiPass();
		std::string					getClientBodyTempPath();
		size_t						getClientMaxBodySize();
};

#endif
