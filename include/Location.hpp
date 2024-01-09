/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 14:07:44 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/09 19:05:14 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATION_HPP
#define LOCATION_HPP

#include "Config.hpp"

class Location {
	protected:
		bool						_autoindex;
		std::string					_root;
		std::vector<std::string>	_index;
		std::vector<std::string>	_limit_except;
		std::map<int, std::string>	_return;

	public:
		Location();
		Location(const Location& other);
		~Location();

		Location& operator=(const Location& other);

		bool						getAutoindex();
		std::string					getRoot();
		std::vector<std::string>	getIndex();
		std::vector<std::string>	getLimitExcept();
		std::map<int, std::string>	getReturn();
};

#endif
