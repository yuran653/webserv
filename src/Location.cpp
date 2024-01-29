/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 14:15:40 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/29 21:53:21 by jgoldste         ###   ########.fr       */
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
	// _cgi_pass(),
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
	// _cgi_pass(other._cgi_pass),
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
		// _cgi_pass = other._cgi_pass;
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

const std::string& Location::getRoot() const {
	return _root;
}

const std::vector<std::string>& Location::getIndex() const {
	return _index;
}

const std::set<std::string>& Location::getLimitExcept() const {
	return _limit_except;
}

const std::pair<int, std::string>& Location::getReturn() const {
	return _return;
}

const std::string& Location::getCgiPass() const {
	return _cgi_pass;
}

const std::string& Location::getClientBodyTempPath() const {
	return _client_body_temp_path;
}

size_t Location::getClientMaxBodySize() const {
	return _client_max_body_size;
}

void Location::_assignBozySize(size_t& start, size_t& finish) {
	(void)start;
	(void)finish;
}

void Location::_assignReturn(size_t& start, size_t& finish) {
	(void)start;
	(void)finish;
}

void Location::_assignLimitExcept(size_t& start, size_t& finish) {
	Config::extractDirective(_location_block, start, finish, LIMIT_EXCEPT);
	std::string limit_exept(_location_block.substr(start, finish - start));
	Config::splitString(_limit_except, limit_exept);
	for (std::set<std::string>::iterator it = _limit_except.begin(); it != _limit_except.end(); it++) {
		for (size_t i = 0; i <= CodesTypes::HTTPmethods.size(); i++) {
			if (i == CodesTypes::HTTPmethods.size())
				throw Config::ReadConfigFileError("Configuration file syntax error: invalid index directive: [" + *it + "]");
			if (it->compare(CodesTypes::HTTPmethods.at(i)) == 0)
				break;
		}
	}
	start = finish;
	for (std::set<std::string>::iterator it = _limit_except.begin(); it != _limit_except.end(); it++)
		std::cout << "Limit except: [" << *it << "]" << std::endl;
}

void Location::_assignIndex (size_t& start, size_t& finish){
	Config::extractDirective(_location_block, start, finish, INDEX);
	std::string index(_location_block.substr(start, finish - start));
	Config::splitString(_index, index);
	if (_index.empty())
		throw Config::ReadConfigFileError("Configuration file syntax error: invalid index directive");
	std::for_each(_index.begin(), _index.end(), Config::validateFileName);
	start = finish;
	for (std::vector<std::string>::iterator it = _index.begin(); it != _index.end(); it++)
		std::cout << "INDEX: [" << *it << "]" << std::endl;
}

void Location::_assignPath(std::string& path, size_t& start, size_t& finish, const std::string& name) {
	Config::extractDirective(_location_block, start, finish, name);
	path = _location_block.substr(start, finish - start);
	Config::extractPath(path);
	Config::validateDirectory(path);
	start = finish;
	std::cout << "ROOT: [" << _root << "]" << std::endl;
}

void Location::_assignAutoindex(size_t& start, size_t& finish) {
	Config::extractDirective(_location_block, start, finish, AUTOINDEX);
	std::string autoindex(_location_block.substr(start, finish - start));
	Config::trimSpaceNonPrintBeginEnd(autoindex);
	if (autoindex.compare(ON) == 0)
		_autoindex = true;
	else if (autoindex.compare(OFF) != 0)
		throw Config::ReadConfigFileError("Configuration file syntax error: invalid autoindex directive");
	start = finish;
	std::cout << "AUTOINDEX: [" << _autoindex << "]" << std::endl;
}

void Location::_caseCgiTempBody(size_t& start, size_t& finish) {
	if (_location_block.compare(start, sizeof(CGI_PASS) - 1, CGI_PASS) == 0)
		_assignPath(_cgi_pass, start, finish, CGI_PASS);
	else if (_location_block.compare(start, sizeof(TEMP_PATH) - 1, TEMP_PATH) == 0)
		_assignPath(_client_body_temp_path, start, finish, TEMP_PATH);
	else if (_location_block.compare(start, sizeof(BODY_SIZE) - 1, BODY_SIZE) == 0)
		_assignBozySize(start, finish);
	else
		throw Config::ReadConfigFileError("Configuration file syntax error: invalid location block directive");
}

void Location::_caseLimit(size_t& start, size_t& finish) {
	if (_location_block.compare(start, sizeof(LIMIT_EXCEPT) - 1, LIMIT_EXCEPT) == 0)
		_assignLimitExcept(start, finish);
	else
		throw Config::ReadConfigFileError("Configuration file syntax error: invalid server block directive");
}

void Location::_caseIndex(size_t& start, size_t& finish) {
	if (_location_block.compare(start, sizeof(INDEX) - 1, INDEX) == 0)
		_assignIndex(start, finish);
	else
		throw Config::ReadConfigFileError("Configuration file syntax error: invalid location block directive");
}

void Location::_caseRootReturn(size_t& start, size_t& finish) {
	if (_location_block.compare(start, sizeof(ROOT) - 1, ROOT) == 0)
		_assignPath(_root, start, finish, ROOT);
	else if (_location_block.compare(start, sizeof(RETURN) - 1, RETURN) == 0)
		_assignReturn(start, finish);
	else
		throw Config::ReadConfigFileError("Configuration file syntax error: invalid location block directive");
}

void Location::_caseAutoindex(size_t& start, size_t& finish) {
	if (_location_block.compare(start, sizeof(AUTOINDEX) - 1, AUTOINDEX) == 0)
		_assignAutoindex(start, finish);
	else
		throw Config::ReadConfigFileError("Configuration file syntax error: invalid location block directive");
}

void	Location::parseLocationBlock() {
	for (size_t start = 0; start < _location_block.size(); start++) {
		Config::skipSpaceNonPrint(_location_block, start);
		if (start >= _location_block.size())
			break;
		size_t finish = start;
		// std::cout << "Case: [" << _location_block.at(start) << "] ->" << std::endl; 
		switch(_location_block.at(start)) {
			case AUTOINDEX_SIGN:
				_caseAutoindex(start, finish);
				break;
			case ROOT_RETURN_SIGN:
				_caseRootReturn(start, finish);
				break;
			case INDEX_SIGN:
				_caseIndex(start, finish);
				break;
			case LSTN_LOC_LIMIT_SIGN:
				_caseLimit(start, finish);
				break;
			case CGI_TEMP_BODY_SIGN:
				_caseCgiTempBody(start, finish);
				break;
			default:
				throw Config::ReadConfigFileError("Configuration file syntax error: invalid directive in location block");
		}
	}
	// !-> check if root directory exists
	// !-> check if location /*.* if cgi_pass is defined
	// _location_block.clear();
}
