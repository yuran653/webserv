#include "Response.hpp"

Response::Response() : _isBodyFile(false), _isCGI(false), _isReady(false) {}

Response::~Response() {
	deleteTempFile();
}

void Response::setConfig(ServerConfig config)
{
	_config = config;
}

bool Response::isReady()
{
	return _isReady;
}

char** Response::initEnv() {
	std::vector<std::string> stringEnvp;
	// std::stringstream ss;
	// ss << request.getBytesRead();
	// stringEnvp.push_back("CONTENT_LENGTH=" + ss.str());
	// ss.clear();
	// if (request.getHeaders().find("content-type") != request.getHeaders().end())
	// 	stringEnvp.push_back("CONTENT_TYPE=" + request.getHeaders().at("content-type"));
	// stringEnvp.push_back("GATEWAY_INTERFACE=CGI/1.1");
	// stringEnvp.push_back("PATH_TRANSLATED=" + request.getPath());
	stringEnvp.push_back("REQUEST_METHOD=" + request.getMethod());
	stringEnvp.push_back("SERVER_PROTOCOL=HTTP/1.1");
	stringEnvp.push_back("PATH_INFO=" + request.getPath());
	// stringEnvp.push_back("QUERY_STRING=" + request.getQuery());
	
	// for (std::map<std::string, std::string>::const_iterator it = request.getHeaders().begin();
	// 	it != request.getHeaders().end(); it++) {
	// 	std::string header = it->first;
	// 	if (it->first == "content-type" || it->first == "content-length")
	// 		continue;
	// 	std::transform(header.begin(), header.end(), header.begin(), ::toupper);
	// 	std::string value = it->second;
	// 	stringEnvp.push_back("HTTP_" + header + "=" + value);
	// }
	char** envp = new char*[stringEnvp.size() + 1];
	for (size_t i = 0; i < stringEnvp.size(); i++) {
		envp[i] = new char[stringEnvp[i].size() + 1];
		strcpy(envp[i], stringEnvp[i].c_str());
	}
	envp[stringEnvp.size()] = NULL;
	return envp;
}

int Response::executeCGI()
{
	_isCGI = true;
	char **env = initEnv();
	_code = CGIInterface::executeCGI(_CGIHeaders, _bodyPath, env, 
		_location.getCgiPass(), request.getTempFilePath());
	// _CGIHeaders = "\r\n\r\n";
	// _bodyPath = "CGI_response.txt";
	std::cout << "******************" << std::endl;
	std::cout << _CGIHeaders << std::endl;
	std::cout << "******************" << std::endl;
	delete[] env;
	_isBodyFile = true;
	return _code;
	// return 200;
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
	_body.append("<html><head><link rel='stylesheet' href='/styles.css'><title>");
	_body.append(pageTitle);
	_body.append("</title></head><body>");
	_body.append(pageBody);
	_body.append("</body></html>");
}

void Response::buildErrorHTMLBody()
{
	std::string errorBody;
	errorBody.append("<div class='header'><div class='project-name'>WEBSERV</div>");
	errorBody.append("<div class='logo'><a href='/'><img alt='Home School 42' ");
	errorBody.append("src='https://42.fr/wp-content/uploads/2021/05/42-Final-sigle-seul.svg'>");
	errorBody.append("</a></div></div><div class='content'><div class='error-code'>");
	errorBody.append(intToString(_code));
	errorBody.append("</div><div class='error-message'>" + CodesTypes::codeMessages.at(_code)+ "</div></div>");
	buildHTML(intToString(_code), errorBody);
}

void Response::buildErrorBody()
{
	std::string filename;
	try {
		filename = _config.getErrorPage().at(_code);
	}
	catch (std::out_of_range &e) {
		buildErrorHTMLBody();
		_bodySize = _body.size();
		return;
	}
	std::ifstream file(filename);
	if (!file.is_open())
		buildErrorHTMLBody();
	else
		_body.assign((std::istreambuf_iterator<char>(file)),
			std::istreambuf_iterator<char>());
	_bodySize = _body.size();
}

bool fileExists(const char* filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}

bool hasReadPermissions(const char* filename) {
    struct stat buffer;
    if (stat(filename, &buffer) == 0) {
        return (buffer.st_mode & S_IRUSR);
    }
    return false;
}

int Response::getFileSize(const std::string &file)
{
	struct stat file_stat;
	if (stat(file.c_str(), &file_stat) != 0)
		return 1;
	_bodySize = file_stat.st_size;
	return 0;
}

int Response::buildFileBody()
{
	if (!fileExists(_fileOrFolder.c_str()))
        return 404;
    else if (!hasReadPermissions(_fileOrFolder.c_str()))
        return 403;
	else
	{
		if (getFileSize(_fileOrFolder))
			return 500;
		if (_bodySize > MAX_FILE_SIZE_FOR_STRING)
		{
			_bodyPath = _fileOrFolder;
			_isBodyFile = true;
		}
		else 
		{
			std::ifstream file(_fileOrFolder);
			_body.assign((std::istreambuf_iterator<char>(file)),
				std::istreambuf_iterator<char>());
		}
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
    if (isDirectory(_fileOrFolder))
		return 403;
    if (std::remove(_fileOrFolder.c_str()) == 0) {
        std::cout << "File deleted successfully.\n" << std::endl;
		return 204;
    } else {
        std::cerr << "Error deleting file" << std::endl;
		return 403;
    }
}

int Response::buildAutoindexBody() {
    std::string indexBody;
    indexBody.append("<div class='header'><div class='project-name'>Index of ");
	indexBody.append(request.getPath());
    indexBody.append("</div><div class='logo'><a href='/'>");
	indexBody.append("<img alt='Home School 42'");
	indexBody.append("src='https://42.fr/wp-content/uploads/2021/05/42-Final-sigle-seul.svg'>");
	indexBody.append("</a></div></div>");
    DIR* dir = opendir(_fileOrFolder.c_str());
    if (!dir) {
        std::cerr << "Error opening directory" << std::endl;
        return 404;
    }
    struct dirent* entry;
    std::string path = request.getPath();
    path = path == "/" ? "" : path;
    indexBody.append("<div class='content'><table class='table'><thead><tr>");
	indexBody.append("<th>Name</th><th>Last Modified</th><th>File Size (bytes)</th></tr></thead><tbody>");
    while ((entry = readdir(dir)) != nullptr) {
		if ((entry->d_name[0] == '.' && strcmp(entry->d_name, "..") != 0) 
		|| strcmp(entry->d_name, ".") == 0)
			continue;
        std::string entryPath = _fileOrFolder + "/" + entry->d_name;
        struct stat fileStat;
        if (stat(entryPath.c_str(), &fileStat) == -1) {
            std::cerr << "Error getting file stat" << std::endl;
            continue;
        }
        std::string classAttribute = S_ISDIR(fileStat.st_mode) ? "class='folder'" : "class='file'";
        std::string name = entry->d_name;
        std::string lastModified = "-";
        std::string fileSize = "-";
        if (S_ISREG(fileStat.st_mode)) {
            size_t size = fileStat.st_size;
            struct tm* modifiedTime = localtime(&fileStat.st_mtime);
            char modifiedTimeString[100];
            strftime(modifiedTimeString, sizeof(modifiedTimeString), "%Y-%m-%d %H:%M:%S", modifiedTime);
            lastModified = modifiedTimeString;
            fileSize = size_tToString(size);
        }
        indexBody.append("<tr " + classAttribute + "><td><a href='" + path + "/" + entry->d_name + 
		"'>" + name + "</a></td><td>" + lastModified + "</td><td>" + fileSize + "</td></tr>");
    }
    indexBody.append("</tbody></table></div>");
    closedir(dir);
    buildHTML("Index of " + request.getPath(), indexBody);
	_bodySize = _body.length();
    return 0;
}

int Response::deleteTempFile()
{
	if (std::remove(request.getTempFilePath().c_str()) != 0)
		return 500;
	else
		return 0;
}

int Response::uploadFile()
{
	if (isDirectory(_fileOrFolder))
		return 403;
	_fileOrFolder = _location.getClientBodyTempPath() + "/" + 
		request.getPath().substr(_location.getLocationPath().length());
	if (rename(request.getTempFilePath().c_str(), _fileOrFolder.c_str()) == 0) {
        std::cout << "File moved successfully." << std::endl;
    } else {
        std::cerr << "Error moving file." << std::endl;
    }
	return 201;
}

void Response::buildStatusLine()
{	
	_headers = "HTTP/1.1 " + intToString(_code) 
		+ " " + CodesTypes::codeMessages.at(_code) + "\r\n";
}

void Response::buildHeaders()
{
	if (_code == 301 || _code == 302)
	{
		_headers += "Location: " + _location.getReturn().second + "\r\n";
		_headers += "Content-Type: text/html\r\n";
		_headers += "Content-Length: " + size_tToString(_bodySize) + "\r\n\r\n";
		return;
	}
	else if (_code >= 400 && _code <= 599)
	{
		_headers += "Content-Type: text/html\r\n";
		_headers += "Content-Length: " + size_tToString(_bodySize) + "\r\n\r\n";
		return;
	}
	else if (_isCGI)
	{
		_headers += _CGIHeaders;
		// std::cout << "+++++++++++++++++++" << std::endl;
		// std::cout << _headers << std::endl;
		// std::cout << "+++++++++++++++++++" << std::endl;
		return;
	}
	std::string MIME;
	size_t pos = _fileOrFolder.find_last_of(".");
	if (pos == std::string::npos || pos == 0)
		MIME = "text/html";
	else
	{
		std::string extension = _fileOrFolder.substr(_fileOrFolder.find_last_of("."));
		try {
			MIME = CodesTypes::MIMEType.at(extension);
		}
		catch (std::out_of_range &e) {
			MIME = "Unknown";
		}
	}
	_headers += "Content-Type: " + MIME + "\r\n";
	_headers += "Content-Length: " + size_tToString(_bodySize) + "\r\n\r\n";
}

int Response::setLocation()
{
	std::string correctLocationURI;
	std::string path = request.getPath();
	std::map<std::string, Location> locations = _config.getLocationMap();
	for (std::map<std::string, Location>::iterator it = locations.begin(); 
		it != locations.end(); it++)
	{
		std::string locationURI = it->first;
		if (locationURI.find("/*.") == 0 && request.getMethod() != "GET")
		// if (locationURI.find("/*.") == 0)
		{
			locationURI = locationURI.substr(2);
			if (path.find(locationURI) == (path.length() - locationURI.length()))
			{
				_location = it->second;
				_fileOrFolder.clear();
				return 0;
			}
			continue;
		}
		if (path.find(locationURI) == 0)
		{
			if (path == locationURI)
			{
				_location = it->second;
				_fileOrFolder = _location.getRoot();
				return 0;
			}
			else if (locationURI == "/"
				|| (path.length() > locationURI.length() 
				&& path[locationURI.length()] == '/'))
			{
					correctLocationURI = locationURI;
			}
		}
	}
	_location = locations.at(correctLocationURI);
	_fileOrFolder = _location.getRoot() + 
		(correctLocationURI == "/" ? path : path.substr(correctLocationURI.length()));
	return 0;
}

int Response::fulfillRequest()
{
	if ((_code = setLocation()))
		return _code;
	std::set<std::string> limitExcept = _location.getLimitExcept();
	if (_location.getReturn().first != -1)
		return _location.getReturn().first;
	if (std::find(limitExcept.begin(), limitExcept.end(), request.getMethod())
		== limitExcept.end())
		return 405;
	if (request.getMethod() == "DELETE")
		return deleteFile();
	if (request.getBytesRead() > _location.getClientMaxBodySize())
		return 413;
	if (_location.getAutoindex() && isDirectory(_fileOrFolder))
	{
		if (buildAutoindexBody())
			return 404;
	}
	else if (!_location.getAutoindex() && isDirectory(_fileOrFolder)
		&& (request.getMethod() == "GET" || request.getMethod() == "HEAD"))
	{
		std::vector<std::string> index = _location.getIndex();
		if (index.empty())
			return 403;
		std::string folderPath = _fileOrFolder;
		for (std::vector<std::string>::iterator it = index.begin(); it != index.end(); it++)
		{
			_fileOrFolder = folderPath + "/" + *it;
			if ((_code = buildFileBody()) == 200)
				return _code;
		}
		return _code;
	}
	else if (!_location.getCgiPass().empty() && request.getMethod() != "GET")
	// else if (!_location.getCgiPass().empty())
		return executeCGI();
	else if (_location.getCgiPass().empty() &&
		(request.getMethod() == "POST" || request.getMethod() == "PUT"))
		return uploadFile();
	else 
		return buildFileBody();
	return 200;
}

// int Response::checkAndModifyCGIHeaders()
// {
// 	size_t pos = _CGIHeaders.find("\r\n\r\n");
// 	if (pos == std::string::npos || pos == 0)
// 		return 1;
// 	size_t posStart = 0;
// 	bool contentTypeExists = false;
// 	pos = _CGIHeaders.find("\r\n");
// 	while (pos != posStart)
// 	{
// 		std::string key, value;
// 		size_t posSC = _CGIHeaders.find(":", posStart);
// 		if (posSC == std::string::npos)
// 			return 1;
// 		key = _CGIHeaders.substr(posStart, posSC);
// 		value = _CGIHeaders.substr(posSC + 1, pos);
// 		if (hasWhiteSpaces(key))
// 			return 1;
// 		toLowerCase(key);
// 		if (key == "content-type")
// 			contentTypeExists = true;
// 		posStart = pos + 2;
// 		pos = _CGIHeaders.find("\r\n", posStart);
// 	}
// 	if (!_body.empty() && !contentTypeExists)
// 		return 1;
// 	_CGIHeaders.insert(_CGIHeaders.find("\r\n\r\n"), "Content-length: " 
// 	+ size_tToString(_bodySize));
// 	return 0;
// }

int Response::checkAndModifyCGIHeaders()
{
	size_t pos = _CGIHeaders.find("\r\n\r\n");
	if (pos == std::string::npos || pos == 0)
		return 1;
	size_t posStart = 0;
	bool contentTypeExists = false;
	pos = _CGIHeaders.find("\r\n");
	std::string header = _CGIHeaders.substr(posStart, pos);
	while (!header.empty() && header != "\r\n")
	{
		std::string key, value;
		size_t posSC = header.find(":");
		if (posSC == std::string::npos)
			return 1;
		key = header.substr(0, posSC);
		value = header.substr(posSC + 1);
		if (hasWhiteSpaces(key))
			return 1;
		toLowerCase(key);
		if (key == "content-type")
			contentTypeExists = true;
		posStart = pos + 2;
		pos = _CGIHeaders.find("\r\n", posStart);
		header = _CGIHeaders.substr(posStart, pos);
	}
	if (!_body.empty() && !contentTypeExists)
		return 1;
	_CGIHeaders.insert(_CGIHeaders.find("\r\n\r\n"), "\r\nContent-length: " 
	+ size_tToString(_bodySize));
	return 0;
}

void Response::buildCGIResponse()
{
	if (getFileSize(_bodyPath))
	{
		_code = 500;
		return;
	}
	if (checkAndModifyCGIHeaders())
	{
		_code = 500;
	}
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
		_code = fulfillRequest();
		if (_code > 399 && _code < 600)
			buildErrorBody();
	}
	if (_isCGI)
	{
		buildCGIResponse();
		if (_code > 399 && _code < 600)
		{
			_CGIHeaders.clear();
			_isBodyFile = false;
			buildErrorBody();
		}
	}
	buildStatusLine();
	buildHeaders();
	_isReady = true;
	return;
}

void Response::sendResponse(int fd)
{
	// char *buff = new char[BUFFSIZE];
	// memset(buff, 0, BUFFSIZE);
	if (!_isBodyFile)
	{
		_headers.append(_body);
		send(fd, _headers.c_str(), _headers.length(), 0);
	}
	else 
	{
		send(fd, _headers.c_str(), _headers.length(), 0);
		size_t size_read = 0;
		int read_fd = open(_bodyPath.c_str(), O_RDONLY);
		char *buff = new char[BUFFSIZE + 1];
		memset(buff, 0, BUFFSIZE + 1);
		while ((size_read = read(read_fd, buff, BUFFSIZE)))
		{
			send(fd, buff, size_read, 0);
			memset(buff, 0, BUFFSIZE);
		}
		delete[] buff;
		std::remove(_bodyPath.c_str());
		std::cout << "REMOVE [" << request.getPath() << "]" << std::endl;
	}
}
