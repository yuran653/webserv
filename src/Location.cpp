/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 14:15:40 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/10 14:06:33 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() :
	_autoindex(false),
	_root(),
	_index(),
	_limit_except(),
	_return(),
	_cgi_pass(""),
	_client_body_temp_path(""),
	_client_max_body_size(0) {
}

Location::Location(const Location& other) :
	_autoindex(other._autoindex),
	_root(other._root),
	_index(other._index.begin(), other._index.end()),
	_limit_except(other._limit_except.begin(), other._limit_except.end()),
	_return(other._return),
	_cgi_pass(""),
	_client_body_temp_path(""),
	_client_max_body_size(0) {
}

Location::~Location() {
}

Location& Location::operator=(const Location& other) {
	if (this != &other) {
		_autoindex = other._autoindex;
		_root = other._root;
		_index = other._index;
		_limit_except = other._limit_except;
		_return = other._return;
		_cgi_pass = other._cgi_pass;
		_client_body_temp_path = other._client_body_temp_path;
		_client_max_body_size = other._client_max_body_size;
	}
	return *this;
}

bool Location::getAutoindex() {

}

std::string Location::getRoot() {

}

std::vector<std::string> Location::getIndex() {

}

std::vector<std::string> Location::getLimitExcept() {

}

std::map<int, std::string> Location::getReturn() {

}
