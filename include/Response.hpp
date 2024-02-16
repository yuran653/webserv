#ifndef RESPONSE_HPP
#define RESPONSE_HPP

#include "CodesTypes.hpp"
#include <iostream>
#include <fstream>
#include <sys/stat.h>
#include <dirent.h>
#include <sys/socket.h>
#include <cstdio>
#include <vector>
#include <algorithm>
#include "Request.hpp"
#include "Config.hpp"
#include "CGIInterface.hpp"
#include "utils.hpp"

enum ResponseStatus
{
	RESPONSE_HEADERS,
	RESPONSE_BODY
};

class Response 
{
	private:
		std::string _fileOrFolder;
		std::string _CGIHeaders;
		std::string _body;
		std::string _bodyPath;
		std::string _headers;
		ServerConfig _config;
		Location _location;
		int		_bodySize;
		bool	_isBodyFile;
		bool	_isCGI;
		bool	_isReady;
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
		int buildFileBody();
		int setLocation();
		int deleteFile();
		int uploadFile();
		int getFileSize(const std::string &file);
		int deleteTempFile();
		char** initEnv();
		int executeCGI();
		std::string getCodeMessage();
		int checkAndModifyCGIHeaders();
		void setConfig(ServerConfig config);
		bool isReady();
		void sendResponse(int fd);
		Request request;
};

#endif
