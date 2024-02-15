#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "CodesTypes.hpp"
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <dirent.h>
#include <cstdio>
#include <vector>
#include <algorithm>
#include "Request.hpp"
#include "Config.hpp"
#include "CGIInterface.hpp"
#include "utils.hpp"

class Response 
{
	private:
		std::string _fileOrFolder;
		std::string _body;
		std::string _CGIHeaders;
		std::string _response;
		ServerConfig _config;
		Location _location;
		bool	_isCGI;
		int _code;

	public:
		Response();
		~Response();
		void buildResponse();
		void buildCGIResponse();
		void buildErrorBody();
		void buildErrorHTMLBody();
		int buildAutoindexBody();
		void buildHTML(const std::string &pageTitle, const std::string &pageBody);
		int fulfillRequest();
		void buildStatusLine();
		void buildHeaders();
		int buildFileBody(std::ifstream &file);
		int setLocation();
		int deleteFile();
		int uploadFile();
		int deleteTempFile();
		char** initEnv();
		int executeCGI();
		std::string getCodeMessage();
		const std::string &getResponse();
		int checkAndModifyCGIHeaders();
		void setConfig(ServerConfig config);
		Request request;
};

#endif
