#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "CodesTypes.hpp"
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <dirent.h>
#include <cstdio>
#include <algorithm>
#include "Request.hpp"
#include "Config.hpp"

class Response 
{
	private:
		std::string _file;
		std::string _body;
		std::string _response;
		ServerConfig _config;
		Location _location;
		bool _cgi;
		int _code;

	public:
		Response();
		void buildResponse();
		void buildErrorBody();
		int buildAutoindexBody();
		void buildHTML(const std::string &pageTitle, const std::string &pageBody);
		int buildBody();
		void buildStatusLine();
		void buildHeaders();
		int buildFileBody(std::ifstream &file);
		int setCorrectPath();
		int deleteFile();
		int uploadFile();
		int executeCGI();
		std::string getCodeMessage();
		void buildDefaultErrorPage();
		const std::string &getResponse();
		void setConfig(ServerConfig config);
		std::vector<std::string> splitPath();
		Request request;
};

#endif
