#include "Response.hpp"

Response::Response()
{
	_code = 0;
}

const std::string &Response::getResponse()
{
	return _response;
}

void Response::setConfig(ServerConfig config)
{
	_config = config;
}

int Response::executeCGI()
{
	// std::vector<std::string> envp;
	// envp.push_back("SERVER_PROTOCOL=HTTP/1.1");
	// envp.push_back("SERVER_SOFTWARE=webserv");
	// // envp.push_back("SERVER_NAME=" + _location.);
	// envp.push_back("REQUEST_METHOD=" + request.getMethod());
	// envp.push_back("REQUEST_URI=" + request.getUri());
	return 201;
}

std::string Response::getCodeMessage()
{
	std::string codeMessage;
	try {
		codeMessage = CodesTypes::codeMessages.at(_code);
	}
	catch (std::out_of_range &e) {
		codeMessage = "Unknown";
	}
	return codeMessage;
}

void Response::buildHTML(const std::string &pageTitle, const std::string &pageBody)
{
	_body = "<html><head><title>" + pageTitle + "</title></head><body>" + pageBody + "</body></html>";
}

void Response::buildErrorBody()
{
	std::string filename;
	try {
		filename = _config.getErrorPage().at(_code);
	}
	catch (std::out_of_range &e) {
		buildHTML(std::to_string(_code), getCodeMessage());
		return;
	}
	std::ifstream file(filename);
	if (!file.is_open())
	{
		buildHTML(std::to_string(_code), getCodeMessage());
	}
	else
		_body.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
}

int Response::buildFileBody(std::ifstream &file)
{
	// if (_code != 200)
	// {
	// 	buildHTML(std::to_string(_code), getCodeMessage());
	// 	return _code;
	// }
	if (!file.is_open())
		return 404;
	else
	{
		if (request.getMethod() == "POST")
			return executeCGI();
		_body.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	}
	return 200;
}

static bool isDirectory(const std::string &path)
{
    struct stat file_stat;
    if (stat(path.c_str(), &file_stat) != 0)
        return (false);

    return (S_ISDIR(file_stat.st_mode));
}

int Response::deleteFile()
{
    if (isDirectory(_file))
		return 403;
    if (std::remove(_file.c_str()) == 0) {
        std::cout << "File deleted successfully.\n" << std::endl;
		return 204;
    } else {
        std::cerr << "Error deleting file" << std::endl;
		return 403;
    }
}

int Response::buildAutoindexBody()
{
	std::string indexBody;
	indexBody = "<h1>Index of " + request.getPath() + "</h1><hr><ul>";
	DIR* dir = opendir(_file.c_str());
    if (!dir) {
        std::cerr << "Error opening directory" << std::endl;
        return 404;
    }
    struct dirent* entry;
    while ((entry = readdir(dir)) != nullptr) {
        indexBody.append("<li><a href=\"" + request.getPath() + "/" + entry->d_name + "\">" + entry->d_name + "</a></li>");
    }
    closedir(dir);
	buildHTML("Index of " + request.getPath(), indexBody);
	return 0;
}

int Response::setCorrectPath()
{
	std::string locationTemp;
	std::string path = request.getPath();
	std::map<std::string, Location> locations = _config.getLocationMap();
	for (std::map<std::string, Location>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		std::string locationPath = it->first;
		if (path.find(locationPath) == 0)
		{
			if (path == locationPath || locationPath == "/" || (path.length() > locationPath.length() &&  path[locationPath.length()] == '/'))
			{
				std::string locationTempNew;
				_location = it->second;
				locationTempNew = it->first;
				std::string finalPath = _location.getRoot() + path.substr(locationPath.length());
				if (locationTempNew.length() > locationTemp.length())
				{
					_location = it->second;
					_file = finalPath;
				}
			}
		}
	}
	// std::vector<std::string> limitExcept = _location.getLimitExcept();
	std::set<std::string> limitExcept = _location.getLimitExcept();
	if (std::find(limitExcept.begin(), limitExcept.end(), request.getMethod()) == limitExcept.end())
		return 405;
	if (_location.getReturn().first != -1)
		_code = _location.getReturn().first;
	return 0;
}

int Response::uploadFile()
{
	return 0;
}

int Response::buildBody()
{
	int code;
	if ((code = setCorrectPath()))
		return code;
	if (request.getMethod() == "DELETE")
		return deleteFile();
	// if (request.getMethod() == "PUT")
	// 	return uploadFile();
	if (request.getBytesRead() > _location.getClientMaxBodySize())
		return 413;
	if (_location.getAutoindex() && isDirectory(_file))
	{
		if (buildAutoindexBody())
			return 404;
	}
	else if (!_location.getAutoindex() && isDirectory(_file) && request.getMethod() == "GET")
	{
		std::vector<std::string> index = _location.getIndex();
		std::string filePath = _file;
		for (std::vector<std::string>::iterator it = index.begin(); it != index.end(); it++)
		{
			_file = filePath + "/" + *it;
			std::ifstream file(_file);
			if ((_code = buildFileBody(file)) == 200)
				return _code;
		}
		return _code;
		// if (_body.empty())
		// 	return 404;
	}
	else 
	{
		std::ifstream file(_file);
		return buildFileBody(file);
	}
	return 200;
}

void Response::buildStatusLine()
{	
	_response = "HTTP/1.1 " + std::to_string(_code) + " " + CodesTypes::codeMessages.at(_code) + "\r\n";
}

void Response::buildHeaders()
{
	if (_code == 301 || _code == 302)
	{
		_response += "Location: " + _location.getReturn().second + "\r\n";
		_response += "Content-Type: text/html\r\n";
		_response += "Content-Length: " + std::to_string(_body.length()) + "\r\n\r\n";
		return;
	}
	std::string MIME;
	size_t pos = _file.find_last_of(".");
	if (pos == std::string::npos || pos == 0)
		MIME = "text/html";
	else
	{
		std::string extension = _file.substr(_file.find_last_of("."));
		try {
			MIME = CodesTypes::MIMEType.at(extension);
		}
		catch (std::out_of_range &e) {
			MIME = "Unknown";
		}
	}
	_response += "Content-Type: " + MIME + "\r\n";
	_response += "Content-Length: " + std::to_string(_body.length()) + "\r\n\r\n";
}

void Response::buildResponse()
{
	_code = request.getErrorCode();
	if (_code == 0)
		_code = 200;
	if (_code != 200)
		buildErrorBody();
	else 
	{
		_code = buildBody();
		if (_code > 399 && _code < 500)
			buildErrorBody();
	}
	buildStatusLine();
	buildHeaders();
	_response += _body;
}
