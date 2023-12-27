#include "Request.hpp"

Request::Request() 
{
	_status = REQUEST_LINE;
}

Request::~Request() {}

Request::Request(const Request &src) 
{
	if (this != &src)
		*this = src;
}

Request &Request::operator=(const Request &src)
{
	_status = src._status;
	return *this;
}

const std::string &Request::getMethod() const
{
	return _method;
}
const std::string &Request::getPath() const
{
	return _path;
}
const std::string &Request::getVersion() const
{
	return _version;
}

const std::string &Request::getQuery() const
{
	return _query;
}

const std::map<std::string, std::string> &Request::getHeaders() const
{
	return _headers;
}

const std::string &Request::getBody() const
{
	return _body;
}

std::ostream &operator<<(std::ostream &os, const Request &request)
{
	std::cout << "Method: " << request.getMethod() << std::endl;
	std::cout << "Path: " << request.getPath() << std::endl;
	std::cout << "Version: " << request.getVersion() << std::endl;
	std::cout << "Query: " << request.getQuery() << std::endl;
	std::cout << "Body:" << request.getBody() << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = request.getHeaders().begin(); it != request.getHeaders().end(); ++it) {
        std::cout << it->first << ":" << it->second << std::endl;
    }
	return os;
}

void throwError(int error)
{
	switch (error)
	{
	case 400:
		std::cout << "400 Bad request" << std::endl;
		break;
	case 403:
		std::cout << "403 Forbidden" << std::endl;
		break;
	case 404:
		std::cout << "404 Not found" << std::endl;
		break;
	case 411:
		std::cout << "411 Length Required" << std::endl;
		break;
	case 414:
		std::cout << "414 Request-URI Too Long" << std::endl;
		break;
	case 501:
		std::cout << "501 Not implemented" << std::endl;
		break;
	case 505:
		std::cout << "505 HTTP Version not supported" << std::endl;
		break;
	default:
		break;
	}
}

bool isPathWithinCurrentFolder(const std::string &path)
{
	if (path.find("/../") == 0)
		return false;
	int depth = 0;
	size_t pos = 1;
	while (pos < path.length())
	{
		size_t nextPos = path.find("/", pos);
		std::string subpath = path.substr(pos, nextPos - pos);
		if (subpath == "..")
		{
			depth--;
			if (depth < 0)
				return false;
		}
		else 
			depth++;
		pos = (nextPos == std::string::npos) ? path.length() : nextPos + 1;
	}
	return true;
}

int Request::parseRequestLine()
{
	size_t pos = _buffer.find_first_not_of("\r\n");  
	_buffer.erase(0, pos);								// removes all the possible new lines at the beginning of the request
	if (_buffer.find("\r\n") != std::string::npos
		|| _buffer.find("\n") != std::string::npos) 	// checks that we have the full first line of the request
	{
		if (_buffer.find("GET") != 0 && _buffer.find("POST") != 0 && _buffer.find("HEAD") != 0) // checks that the method is the very first thing after all new lines
			return 400;
		std::istringstream buffer(_buffer);
		buffer >> _method >> _path >> _version;			// parses the first line of the request
		if (_method != "GET" && _method != "POST" && _method != "HEAD")
			return 501;
		if (_path.length() > MAX_PATH_LENGTH)
			return 414;
		if (_path.find("http://") == 0)					// checks if there's full path
		{
			size_t pos = _path.find("/", 7);
			_headers["host"] = _path.substr(7, pos - 7);
			if ((pos = _headers["host"].find(":")) != std::string::npos) // checks if there's a port number
			{
				if (_headers["host"].substr(pos) != ":80")
					return 400;
				_headers["host"].erase(pos);
				_path.erase(0, pos + 10);
			}
			else 
				_path.erase(0, 7 + _headers["host"].length());
			if (_path.length() == 0)
				_path = "/";
		}
		if (_path[0] == '/') 				// checks that relative path starts with /
		{
			if (_path.find("?") != std::string::npos)
			{
				_query = _path.substr(_path.find("?") + 1);
				_path.erase(_path.find("?"));
			}
			if (!isPathWithinCurrentFolder(_path)) // checks that there is no path exploit
				return 403;
		}
		else 
			return 400;
		if (_version.find("HTTP/") != 0)
			return 400;
		else if (_version != "HTTP/1.1" && _version != "HTTP/1.0")
			return 505;
		_version.erase(0, 5);

		size_t ptrIdx = buffer.tellg(); 			// checks there is new line right after HTTP version and erases the first line from the buffer
		if (_buffer.find("\r\n", ptrIdx) == ptrIdx)
			_buffer.erase(0, _buffer.find("\r\n") + 2);
		else if (_buffer.find("\n", ptrIdx) == ptrIdx)
			_buffer.erase(0, _buffer.find("\n") + 1);
		else
			return 400;

		_status = HEADERS;
	}
	return 0;
}

bool hasWhiteSpaces(const std::string &str)
{
	for (size_t i = 0; i < str.length(); i++)
		if (std::isspace(str[i]))
			return true;
	return false;
}

void removeWhiteSpaces(std::string &str)
{
	size_t pos = str.find_first_not_of(" \t");
	str.erase(0, pos);
	pos = str.find_last_not_of(" \t");
	str.erase(pos + 1);
}

void toLowerCase(std::string &str)
{
	for (size_t i = 0; i < str.length(); i++)
		str[i] = std::tolower(str[i]);
}

int Request::parseHeaders()
{
	size_t pos = 0;
	size_t posN =_buffer.find("\n");
	size_t posRN = _buffer.find("\r\n");

	while (posN != std::string::npos
		|| posRN != std::string::npos) 	// checks that we have the full line of header field
	{
		pos = posN < posRN ? posN : posRN;
		int a = posN < posRN ? 1 : 2;
		size_t posColon = _buffer.find(":");
		if (posColon == std::string::npos)
			return 400;
		std::string key = _buffer.substr(0, posColon);
		if (hasWhiteSpaces(key))
			return 400;
		std::string value = _buffer.substr(posColon + 1, pos - posColon - 1);
		removeWhiteSpaces(value);
		toLowerCase(key);
		if (_headers.find(key) != _headers.end())
			return 400;
		_headers[key] = value;
		_buffer.erase(0, pos + a);
		posN =_buffer.find("\n");
		posRN = _buffer.find("\r\n");
		if (posN == 0 || posRN == 0) 
		{
			a = posN < posRN ? 1 : 2;
			_buffer.erase(0, a);
			if (_headers.find("host") == _headers.end())
				return 400;
			if (_method == "POST")
				_status = PREBODY;
			else if (_buffer.length() > 0)
				return 400;
			break;
		}
	}
	return 0;
}

int Request::beforeParseBody()
{
	if (_headers.find("transfer-encoding") != _headers.end() 
	&& _headers["transfer-encoding"] != "identity")
	{
		_status = CHUNKS;
		return 0;
	}
	if (_headers.find("content-length") == _headers.end())
		return 411;
	else if(_headers["content-length"].find_first_not_of("0123456789") != std::string::npos || 
	std::stoll(_headers["content-length"]) < 0)
		return 400;
	_bodySize = static_cast<size_t>(std::stoll(_headers["content-length"]));
	_status = BODY;
	return 0;
}

int Request::parseBody()
{
	// if ((_buffer[_buffer.length() - 2] == '\r' && 
	// _buffer[_buffer.length() - 1] == '\n' &&
	// _buffer.length() + _body.length() > _bodySize - 2) ||
	// 	(_buffer[_buffer.length() - 1] == '\n' &&
	// _buffer.length() + _body.length() > _bodySize - 1))
	// 	return 400;
	_body += _buffer;
	_buffer.clear();
	return 0;
}

int Request::parseChunks()
{
	return 1;
}

int Request::parse(const std::string &requestChunk)
{
	_buffer += requestChunk;
	int ret = 0;
	if (_status == REQUEST_LINE)
		ret = parseRequestLine();	// parse the first line of the request
	if (_status == HEADERS)
		ret = parseHeaders();		// parse the headers of the request
	if (_status == PREBODY)
		ret = beforeParseBody();			// parse the body of the request
	if (_status == BODY)
		ret = parseBody();			// parse the body of the request
	if (_status == CHUNKS)
		ret = parseChunks();		// parse chunks of the body of the request
	if (ret > 1)
	{
		throwError(ret);
		return 1;
	}
	if (ret == 1)
		return 1;
	return 0;
}