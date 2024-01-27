/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 14:15:40 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/27 15:27:05 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() :
	_location_block(),
	_autoindex(false),
	_root(),
	_index(),
	_limit_except(),
	_return(std::make_pair(-1, "")),
	_cgi_pass(),
	_client_body_temp_path(),
	_client_max_body_size(10 * std::pow(2, 20)) {
}

Location::Location(const Location& other) :
	_location_block(other._location_block),
	_autoindex(other._autoindex),
	_root(other._root),
	_index(other._index.begin(), other._index.end()),
	_limit_except(other._limit_except.begin(), other._limit_except.end()),
	_return(other._return),
	_cgi_pass(other._cgi_pass),
	_client_body_temp_path(other._client_body_temp_path),
	_client_max_body_size(other._client_max_body_size) {
}

Location::~Location() {
}

Location& Location::operator=(const Location& other) {
	if (this != &other) {
		_location_block = other._location_block;
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

std::string& Location::getLocationBlock() {
	return _location_block;
}

const std::string& Location::getLocationBlock() const {
	return _location_block;
}

bool Location::getAutoindex() const {
	return _autoindex;
}

std::string Location::getRoot() const {
	return _root;
}

std::vector<std::string> Location::getIndex() const {
	return _index;
}

std::vector<std::string> Location::getLimitExcept() const {
	return _limit_except;
}

std::pair<int, std::string> Location::getReturn() const {
	return _return;
}

std::string Location::getCgiPass() const {
	return _cgi_pass;
}

std::string Location::getClientBodyTempPath() const {
	return _client_body_temp_path;
}

size_t Location::getClientMaxBodySize() const {
	return _client_max_body_size;
}

void	Location::parseLocationBlock() {
	std::cout << "Location block:" << std::endl << _location_block << std::endl;
	// _location_block.clear();
}
