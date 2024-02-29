/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Config.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/28 18:40:27 by jgoldste          #+#    #+#             */
/*   Updated: 2024/02/04 20:44:03 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Config.hpp"

std::string	Config::_config_content;
std::string	Config::_buffer;

Config::Config() {
}

Config::~Config() {
}

unsigned int Config::_fileCounter = 1;

int Config::createTempFile(std::string& path, int& fd)
{
	std::stringstream ss;
	ss << "temp_" << Config::_fileCounter;
	Config::_fileCounter++;
	path += ss.str();
	fd = open(path.c_str(), O_WRONLY | O_CREAT | O_TRUNC | O_EXCL, 
		S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (fd == -1)
		return 500;
	if (fcntl(fd, F_SETFL, O_NONBLOCK) == -1) {
		close(fd);
		return 500;
	}
	return 0;
}

void Config::validateFileName(const std::string& file_name) {
	for (size_t i = 0; i < file_name.size(); i++)
		if (!((file_name.at(i) >= 45 && file_name.at(i) <= 57)
			|| (file_name.at(i) >= 65 && file_name.at(i) <= 90)
			|| (file_name.at(i) == 95)
			|| (file_name.at(i) >= 97 && file_name.at(i) <= 122)))
			throw ReadConfigFileError("Configuration file syntax error: file name contains forbidden character(s): [" + file_name + "]");
}

void Config::validateDirectory(const std::string& path) {
	std::string tmp_path(path);
	tmp_path.insert(tmp_path.begin(), DOT);
	DIR* dir = opendir(tmp_path.c_str());
	if (dir)
		closedir(dir);
	else
		throw ReadConfigFileError("Configuration file syntax error: directory does not exist: [" + path + "]");
}

void Config::extractValidateCode(const std::string& code_str, int& code, const int& min, const int& max) {
	std::istringstream(code_str) >> code;
	if (code < min || code > max || CodesTypes::codeMessages.find(code) == CodesTypes::codeMessages.end())
		throw Config::ReadConfigFileError("Configuration file syntax error: invalid error code: [" + code_str + "]");
}

void Config::validateFile(const std::string& file_name) {
	std::ifstream is(file_name);
	if (is.is_open())
		is.close();
	else
		throw ReadConfigFileError("Configuration file syntax error: [" + file_name + "]: not exist or invalid access rights");
}

void Config::checkRemoveSlash(std::string& path) {
	if (path.at(0) != SLASH_SIGN)
		throw Config::ReadConfigFileError("Configuration file syntax error: invalid path: [" + path + "]");
	if (path.size() > 2 && path.at(path.size() - 1) == SLASH_SIGN)
		path.erase(path.size() - 1, 1);
	if (path.size() > 1 && path.at(path.size() - 1) == SLASH_SIGN)
		throw ReadConfigFileError("Configuration file syntax error: invalid path: [" + path + "]");
	for (std::string::iterator it = path.begin(); it != path.end(); it++)
		if (*it == SLASH_SIGN && *(it + 1) == SLASH_SIGN
			&& (path.compare(HTTP_STR) != 0 || path.compare(HTTPS_STR) != 0))
			throw ReadConfigFileError("Configuration file syntax error: invalid path: [" + path + "]");
}

void Config::checkSpacesNonPrint(const std::string& path) {
	for (size_t i = 0; i < path.size(); i++)
		if (path.at(i) == SPACE_SIGN || path.at(i) == QUESTION_SIGN|| std::isprint(path.at(i)) == 0)
			throw ReadConfigFileError("Configuration file syntax error: forbidden character ["
				+ (std::string(1, path.at(i))) + "] in the path [" + path + "]");
}

void Config::extractPath(std::string& path) {
	Config::trimSpaceNonPrintBeginEnd(path);
	Config::checkSpacesNonPrint(path);
	Config::checkRemoveSlash(path);
}

void Config::isDigitString(const std::string& str,
	const size_t& start, const size_t& finish, const std::string& error_message) {
	for (size_t i = start; i < str.size() && i < finish; i++)
		if (isdigit(str.at(i)) == 0)
			throw ReadConfigFileError(error_message);
}

void Config::splitString(std::set<std::string>& str_set, const std::string& str) {
	std::istringstream stream(str);
	while (stream.tellg() != -1) {
		std::string token;
		stream >> token;
		if (str_set.insert(token).second == false)
			throw ReadConfigFileError("Configuration file syntax error: directive duplication: [" + token + "]");
		token.clear();
	}
}

void Config::splitString(std::vector<std::string>& str_vector, const std::string& str) {
	std::istringstream stream(str);
	while (stream.tellg() != -1) {
		std::string token;
		stream >> token;
		if (token.empty() == false)
			str_vector.push_back(token);
		token.clear();
	}
}

void Config::trimSpaceNonPrintBeginEnd(std::string& content) {
	trimSpaceNonPrintBegin(content);
	trimSpaceNonPrintEnd(content);
}

void Config::trimSpaceNonPrintEnd(std::string& content) {
	if (!content.size())
		return;
	size_t i = content.size() - 1;
	while (i >= 0 && (content.at(i) == SPACE_SIGN || std::isprint(content.at(i)) == 0))
		i--;
	content.erase(i + 1, content.size() - i);
}

void Config::trimSpaceNonPrintBegin(std::string& content) {
	if (!content.size())
		return;
	size_t i = 0;
	while (i < content.size() && (content.at(i) == SPACE_SIGN || std::isprint(content.at(i)) == 0))
		i++;
	content.erase(0, i);
}

void Config::extractDirective(const std::string& content, size_t& start, size_t& finish, const std::string& name) {
	start += name.size();
	finish = content.find_first_of(END_DIRECTIVE_SIGN, start);
	if (finish == std::string::npos)
		throw ReadConfigFileError("Configuration file syntax error: invalid " + name + "directive");
}

void Config::extractCodePath(std::string& path, int& code,
	const int& min, const int& max, const std::string& name) {
	std::vector<std::string> code_path;
	splitString(code_path, path);
	path.clear();
	if (code_path.size() != 2) {
		throw Config::ReadConfigFileError
			("Configuration file syntax error: invalid " + name + "directive");}
 	if (code_path.at(0).size() != 3) {
		throw Config::ReadConfigFileError
			("Configuration file syntax error: invalid " + name + "parameter: [" + code_path.at(0) + "]");}
	isDigitString(code_path.at(0), 0, code_path.size(),
		("Configuration file syntax error: invalid " + name +"directive"));
	extractValidateCode(code_path.at(0), code, min, max);
	path = code_path.at(1);
	switch(path.at(0)) {
		case SLASH_SIGN:
			extractPath(path);
			path.insert(path.begin(), DOT);
			break;
		case HTTP_SIGN:
			if (path.compare(0, sizeof(HTTP_STR) - 1, HTTP_STR) != 0
				&& path.compare(0, sizeof(HTTPS_STR) - 1, HTTPS_STR) != 0) {
				throw Config::ReadConfigFileError
					("Configuration file syntax error: invalid " + name + "directive: [" + path + "]");}
				checkSpacesNonPrint(path);
			break;
		default:
			throw Config::ReadConfigFileError
				("Configuration file syntax error: invalid " + name + "directive: [" + path + "]");
	}
	code_path.clear();
}

void Config::skipSpaceNonPrint(const std::string& content, size_t& i) {
	while (i < content.size() && (content.at(i) == SPACE_SIGN || std::isprint(content.at(i)) == 0))
		i++;
}

void Config::_checkServerDuplication(const std::vector<ServerConfig>& server_config) {
	for (size_t i = 0; i < server_config.size(); i++)
		for (size_t j = i + 1; j < server_config.size(); j++)
			if (server_config[i].getListen() == server_config[j].getListen()
				&& server_config[i].getServerName() == server_config[j].getServerName()) {
				std::ostringstream oss;
				oss << server_config[i].getListen().second;
				std::string server_names;
				for (size_t idx = 0; idx < server_config[i].getServerName().size(); idx++) {
					server_names += server_config[i].getServerName()[idx];
					if (idx != server_config[i].getServerName().size() - 1)
						server_names += " ";
				}
				throw ReadConfigFileError("Configuration file syntax error: duplications of defined servers: "
					+ server_config[i].getListen().first +  ":" + oss.str() + " " + server_names);
			}
}

void Config::_checkRootLocation(const ServerConfig& server_config) {
	if (server_config.getLocationMap().find(ROOT_LOCATION) == server_config.getLocationMap().end())
		throw ReadConfigFileError("Configuration file syntax error: root location does not defined in the server");
}

void Config::_addServerBlock(std::vector<ServerConfig>& server_config, size_t& start, size_t& finish) {
	ServerConfig config;
	config.getServerBlock() = _config_content.substr(start, finish - start);
	if (config.getServerBlock().empty())
		throw Config::ReadConfigFileError("Configuration file syntax error: server block does not defined");
	server_config.push_back(config);
	start = finish;
}

void Config::bracesValidation(const std::string& content, size_t& start, size_t& finish) {
	if (start > content.size() - 1 || content.at(start) != BLOCK_OPEN_SIGN)
		throw ReadConfigFileError("Configuration file syntax error: invalid braces");
	start++;
	finish = start;
	size_t braces_not_closed = 1;
	while (finish < content.size() && braces_not_closed) {
		finish++;
		if (content.at(finish) == BLOCK_OPEN_SIGN)
			braces_not_closed++;
		if (content.at(finish) == BLOCK_CLOSE_SIGN)
			braces_not_closed--;
	}
	if (braces_not_closed != 0)
		throw ReadConfigFileError("Configuration file syntax error: invalid braces");
}

void Config::_extractServerBlocks(std::vector<ServerConfig>& config_type) {
	for (size_t start = 0; start < _config_content.size(); start++) {
		try {
			skipSpaceNonPrint(_config_content, start);
			if (start < _config_content.size()
				&& _config_content.compare(start, sizeof(SERVER_BLOCK) - 1, SERVER_BLOCK) == 0) {
				start += sizeof(SERVER_BLOCK) - 1;
				skipSpaceNonPrint(_config_content, start);
				size_t finish = start;
				bracesValidation(_config_content, start, finish);
				_addServerBlock(config_type, start, finish);
			} else {
				if (config_type.size() == 0)
					throw ReadConfigFileError("Configuration file syntax error: not possible to define server(s) block(s)");
				skipSpaceNonPrint(_config_content, start);
				if (start != _config_content.size())
					throw ReadConfigFileError("Configuration file syntax error: invalid braces");
			}
		} catch (const std::out_of_range& e) {
			break;
		}
	}
}

void Config::_removeComments(std::string& content) {
	for (size_t start = 0; start < content.size(); start++) {
		if (content.at(start) == COMMENT_SIGN) {
			size_t finish = content.find_first_of(NEW_LINE_SIGN, start);
			if (finish == std::string::npos)
				finish = content.size();
			content.erase(start, finish - start);
		}
	}
}

void Config::_readConfigContent(const std::string& config_name) {
	static std::fstream is(config_name);
	if (is.is_open() == false)
		throw ReadConfigFileError("Can not open configuration file [" + config_name + "]");
	while (std::getline(is, _buffer, EOT))
		_config_content += _buffer;
	is.close();
	_removeComments(_config_content);
	trimSpaceNonPrintBeginEnd(_config_content);
	if (_config_content.empty())
		throw ReadConfigFileError("The configuration file [" + config_name + "] is empty");
}

void Config::createServerConfig(const std::string& config_name,
	std::vector<ServerConfig>& server_config) {
	_readConfigContent(config_name);
	_extractServerBlocks(server_config);
	for (std::vector<ServerConfig>::iterator it = server_config.begin(); it != server_config.end(); it++) {
		it->parseServerBlock();
		_checkRootLocation(*it);
	}
	_checkServerDuplication(server_config);
	_config_content.clear();
	_buffer.clear();
}