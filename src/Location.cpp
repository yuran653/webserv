/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 14:15:40 by jgoldste          #+#    #+#             */
/*   Updated: 2024/02/15 15:15:00 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

Location::Location() :
	_location_block(),
	_location_path(),
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
	_location_path(other._location_path),
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
		_location_path = other._location_path;
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

const std::string& Location::getLocationPath() const {
	return _location_path;
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

ssize_t Location::getClientMaxBodySize() const {
	return _client_max_body_size;
}

void Location::_validateReturnLimitExcept() {
	if (_limit_except.empty())
		if (_return.first == -1 && _cgi_pass.empty())
			throw Config::ReadConfigFileError ("Configuration file syntax error: if "
				+ (std::string(LIMIT_EXCEPT)) + "undefined, " + (std::string(RETURN))
				+ "or " + (std::string(CGI_PASS)) + "should be defined");
}

void Location::_validateTempCGIPath() {
	if (_location_path.compare(0, sizeof(WILDCARD_PATH) - 1, WILDCARD_PATH) == 0
		&& _cgi_pass.empty())
		throw Config::ReadConfigFileError
			("Configuration file syntax error: for wildcard path [" + _location_path
				+ "] " + (std::string(CGI_PASS)) + "should be defined");
	if (_cgi_pass.empty() == false
		&& (_index.empty() == false || _root.empty() == false || _return.first != -1))
		throw Config::ReadConfigFileError
			("Configuration file syntax error: simultaneous definition of " + (std::string(CGI_PASS))
				+ "and " + (std::string(INDEX)) + "/ " + (std::string(ROOT)) + "/ "
				+ (std::string(RETURN)) + "is not allowed");
	for (std::set<std::string>::iterator it = _limit_except.begin(); it != _limit_except.end(); it++)
		if ((it->compare("POST") == 0 || it->compare("PUT") == 0)
			&& _client_body_temp_path.empty() && _cgi_pass.empty())
			throw Config::ReadConfigFileError
				("Configuration file syntax error: " + *it + " requires " + (std::string(TEMP_PATH))
					+ "or " + (std::string(CGI_PASS)) + "to be defined");
}

void Location::_validateRoot() {
	if (_root.empty())
		if (_return.first == -1 && _cgi_pass.empty())
			throw Config::ReadConfigFileError
				("Configuration file syntax error: root and/or return directives are not defined");
}

void Location::_validateBodySize(const std::string& body_size_str, const size_t& multiplier) {
	Config::isDigitString(body_size_str, 0, body_size_str.size() -1,
		"Configuration file syntax error: invalid " + (std::string)BODY_SIZE + "parameter");
	std::istringstream(body_size_str) >> _client_max_body_size;
	_client_max_body_size *= multiplier;
	if (_client_max_body_size > 10 * std::pow(2, 30))
		throw Config::ReadConfigFileError("Configuration file syntax error: " +
			(std::string)BODY_SIZE + "exceeds max size");
}

void Location::_setTrimMultiplier(std::string& body_size_str, size_t& multiplier, const size_t& exponent) {
	multiplier = std::pow(2, exponent);
	body_size_str.erase(body_size_str.size() - 1, 1);
}

void Location::_assignBozySize(size_t& start, size_t& finish) {
	Config::extractDirective(_location_block, start, finish, BODY_SIZE);
	std::string body_size_str(_location_block.substr(start, finish - start));
	Config::trimSpaceNonPrintBeginEnd(body_size_str);
	if (body_size_str.empty())
		throw Config::ReadConfigFileError("Configuration file syntax error: "
			+ (std::string)BODY_SIZE + "directive is not defined");
	start = finish;
	Config::trimSpaceNonPrintBeginEnd(body_size_str);
	size_t multiplier = 1;
	if (std::isdigit(*(body_size_str.rbegin())) == 0) {
		switch (*(body_size_str.rbegin())) {
			case KB:
				_setTrimMultiplier(body_size_str, multiplier, 10);
				break;
			case MB:
				_setTrimMultiplier(body_size_str, multiplier, 20);
				break;
			case GB:
				_setTrimMultiplier(body_size_str, multiplier, 30);
				break;
			default:
				throw Config::ReadConfigFileError("Configuration file syntax error: invalid "
					+ (std::string)BODY_SIZE + "parameter");
		}
	}
	_validateBodySize(body_size_str, multiplier);
}

void Location::_assignReturn(size_t& start, size_t& finish) {
	Config::extractDirective(_location_block, start, finish, RETURN);
	std::string return_path(_location_block.substr(start, finish - start));
	Config::trimSpaceNonPrintBeginEnd(return_path);
	if (return_path.empty())
		throw Config::ReadConfigFileError("Configuration file syntax error: " +
			(std::string)RETURN + "directive does not defined");
	start = finish;
	int return_code;
	Config::extractCodePath(return_path, return_code, MIN_CODE, MAX_CODE, RETURN);
	_return = std::make_pair(return_code, return_path);
}

void Location::_assignLimitExcept(size_t& start, size_t& finish) {
	Config::extractDirective(_location_block, start, finish, LIMIT_EXCEPT);
	std::string limit_exept(_location_block.substr(start, finish - start));
	Config::trimSpaceNonPrintBeginEnd(limit_exept);
	if (limit_exept.empty())
		throw Config::ReadConfigFileError("Configuration file syntax error: "
			+ (std::string)LIMIT_EXCEPT + "parameter is not defined");
	start = finish;
	Config::splitString(_limit_except, limit_exept);
	for (std::set<std::string>::iterator it = _limit_except.begin(); it != _limit_except.end(); it++)
		if (CodesTypes::HTTPMethods.find(*it) == CodesTypes::HTTPMethods.end())
			throw Config::ReadConfigFileError("Configuration file syntax error: invalid index directive: [" + *it + "]");
}

void Location::_assignIndex (size_t& start, size_t& finish){
	Config::extractDirective(_location_block, start, finish, INDEX);
	std::string index(_location_block.substr(start, finish - start));
	Config::trimSpaceNonPrintBeginEnd(index);
	if (index.empty())
		throw Config::ReadConfigFileError("Configuration file syntax error: " +
			(std::string)INDEX + "directive is not defined");
	start = finish;
	Config::splitString(_index, index);
	if (_index.empty())
		throw Config::ReadConfigFileError("Configuration file syntax error: invalid index directive");
	std::for_each(_index.begin(), _index.end(), Config::validateFileName);
}

void Location::_assignPath(std::string& path, size_t& start, size_t& finish, const std::string& name) {
	Config::extractDirective(_location_block, start, finish, name);
	path = _location_block.substr(start, finish - start);
	Config::trimSpaceNonPrintBeginEnd(path);
	if (path.empty())
		throw Config::ReadConfigFileError("Configuration file syntax error: "
			+ name + " directive parameter is not defined");
	start = finish;
	Config::extractPath(path);
	Config::validateDirectory(path);
	path.insert(path.begin(), DOT);
}

void Location::_assignFilePath(std::string& path, size_t& start, size_t& finish, const std::string& name) {
	Config::extractDirective(_location_block, start, finish, name);
	path = _location_block.substr(start, finish - start);
	Config::trimSpaceNonPrintBeginEnd(path);
	if (path.empty())
		throw Config::ReadConfigFileError("Configuration file syntax error: "
			+ name + " directive parameter is not defined");
	start = finish;
	Config::extractPath(path);
	path.insert(path.begin(), DOT);
	Config::validateFile(path);
}

void Location::_assignAutoindex(size_t& start, size_t& finish) {
	Config::extractDirective(_location_block, start, finish, AUTOINDEX);
	std::string autoindex(_location_block.substr(start, finish - start));
	Config::trimSpaceNonPrintBeginEnd(autoindex);
	if (autoindex.empty())
		throw Config::ReadConfigFileError("Configuration file syntax error: "
			+ (std::string)AUTOINDEX + "directive is not defined");
	start = finish;
	if (autoindex.compare(ON) == 0)
		_autoindex = true;
	else if (autoindex.compare(OFF) != 0)
		throw Config::ReadConfigFileError("Configuration file syntax error: invalid autoindex directive");
}

void Location::_caseCgiTempBody(size_t& start, size_t& finish) {
	if (_location_block.compare(start, sizeof(CGI_PASS) - 1, CGI_PASS) == 0)
		_assignFilePath(_cgi_pass, start, finish, CGI_PASS);
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

void	Location::parseLocationBlock(const std::string& path) {
	_location_path = path;
	for (size_t start = 0; start < _location_block.size(); start++) {
		Config::skipSpaceNonPrint(_location_block, start);
		if (start >= _location_block.size())
			break;
		size_t finish = start;
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
	_location_block.clear();
	_validateTempCGIPath();
	_validateRoot();
	_validateReturnLimitExcept();
}

	// пробелы все
	// два слэша
	// минус слэш в конце
	// точка в руте