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
	RESPONSE_BODY,
	SENT
};

class Response 
{
	private:
		std::string _fileOrFolder;
		std::string _CGIHeaders;
		std::string _body;
		std::string _bodyPath;
		std::string _headers;
		ServerConfig &_config;
		ResponseStatus _status;
		Location _location;
		int		_bodySize;
		int 	_bodyFd;
		bool	_isBodyFile;
		bool	_isCGI;
		bool	_isReady;
		int 	_code;
		ssize_t _bytesSent;

	public:
		Response(Request &request, ServerConfig &config);
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
		void setLocation();
		int getIndex();
		int deleteFile();
		int uploadFile();
		int getFileSize(const std::string &file);
		void deleteTempFiles();
		char** initEnv();
		int executeCGI();
		std::string getCodeMessage();
		int checkAndModifyCGIHeaders();
		bool isReady();
		bool isSent();
		int sendResponse(int fd);
		int sendHeaders(int fd);
		int sendBody(int fd);
		Request &request;
};

#endif
