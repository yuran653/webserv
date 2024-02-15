#include "Request.hpp"

Request::Request()
{
	_status = REQUEST_LINE;
	_chunkStatus = CHUNK_SIZE;
	_bytesRead = 0;
	_tempFileFd = -1;
}

Request::~Request()
{
	if (_tempFileFd != -1)
		close(_tempFileFd);
}

Request::Request(const Request &src)
{
	*this = src;
}

Request &Request::operator=(const Request &src)
{
	if (this != &src)
	{
		_method = src._method;
		_path = src._path;
		_version = src._version;
		_query = src._query;
		_headers = src._headers;
		_tempFileFd = src._tempFileFd;
		_tempFilePath = src._tempFilePath;
		_chunkSize = src._chunkSize;
		_bodySize = src._bodySize;
		_bytesRead = src._bytesRead;
		_buffer = src._buffer;
		_bodyBuffer = src._bodyBuffer;
		_errorCode = src._errorCode;
		_status = src._status;
		_chunkStatus = src._chunkStatus;
	}
	return *this;
}

bool Request::isParsingComplete() const
{
	return (_status == DONE);
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
int Request::getTempFileFd() const
{
	return _tempFileFd;
}
const std::string &Request::getTempFilePath() const
{
	return _tempFilePath;
}
ssize_t Request::getBytesRead() const
{
	return _bytesRead;
}
int Request::getErrorCode() const
{
	return _errorCode;
}
void Request::setTempFileFd(int tempFileFd) 
{
	_tempFileFd = tempFileFd;
}


std::ostream &operator<<(std::ostream &os, const Request &request)
{
	std::cout << "Method: " << request.getMethod() << std::endl;
	std::cout << "Path: " << request.getPath() << std::endl;
	std::cout << "Version: " << request.getVersion() << std::endl;
	std::cout << "Query: " << request.getQuery() << std::endl;
	std::cout << "FD: " << request.getTempFileFd() << std::endl;
	std::cout << "Temp File Path: " << request.getTempFilePath() << std::endl;
	std::cout << "Error: " << request.getErrorCode() << std::endl;
	for (std::map<std::string, std::string>::const_iterator it = request.getHeaders().begin();
		it != request.getHeaders().end(); ++it)
	{
		std::cout << it->first << ":" << it->second << std::endl;
	}
	return os;
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

void Request::simplifyPath()
{
	std::istringstream iss(_path);
	std::vector<std::string> tokens;
	std::string token;

	// Split the path into tokens
	while (std::getline(iss, token, '/'))
	{
		if (!token.empty())
		{
			if (token == ".." && !tokens.empty())
				tokens.pop_back();
			else if (token != "..")
				tokens.push_back(token);
		}
	}
	// Reconstruct the simplified path
	std::stack<std::string> dirs;
	for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); it++)
		dirs.push(*it);
	std::string simplifiedPath;
	while (!dirs.empty())
	{
		simplifiedPath = '/' + dirs.top() + simplifiedPath;
		dirs.pop();
	}
	// Handle the case when the path is empty (root directory)
	_path = simplifiedPath.empty() ? "/" : simplifiedPath;
}

void Request::removeCurrentDirDots()
{
	size_t pos;
	while ((pos = _path.find("/./")) != std::string::npos)
		_path.erase(pos, 2);
}

int Request::createTempFile()
{
	uuid_t uuid;
	uuid_generate(uuid);
	char uuid_str[37];
	uuid_unparse(uuid, uuid_str);

	std::stringstream ss;
	ss << "temp_" << uuid_str;
	_tempFilePath = ss.str();
	_tempFileFd = open(_tempFilePath.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 
		S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	if (_tempFileFd == -1)
		return 500;
	return 0;
}

int Request::writeToFile()
{
	ssize_t bytesWritten = write(_tempFileFd, _bodyBuffer.c_str(), _bodyBuffer.size());
	_bodyBuffer.clear();
	if (bytesWritten == -1)
	{
		std::cout << "Error writing to file" << std::endl;
		return 1;
	}
	return 0;
}

int Request::parseRequestLine()
{
	size_t pos = _buffer.find_first_not_of("\r\n");
	_buffer.erase(0, pos);
	if (_buffer.find("\r\n") != std::string::npos)
	{
		std::istringstream buffer(_buffer);
		buffer >> _method;
		std::set<std::string>::iterator it = CodesTypes::HTTPMethods.find(_method);
		if (it == CodesTypes::HTTPMethods.end())
			return 501;
		buffer >> _path >> _version;
		if (_path.length() > MAX_PATH_LENGTH)
			return 414;
		if (_path[0] != '/')
			return 400;
		else
		{
			if (_path.find("?") != std::string::npos)
			{
				_query = _path.substr(_path.find("?") + 1);
				_path.erase(_path.find("?"));
			}
			removeCurrentDirDots();
			if (!isPathWithinCurrentFolder(_path))
				return 403;
			else
				simplifyPath();
		}
		if (_version.find("HTTP/") != 0)
			return 400;
		else if (_version != "HTTP/1.1" && _version != "HTTP/1.0")
			return 505;
		_buffer.erase(0, _buffer.find("\r\n") + 2);
		_status = HEADERS;
	}
	return 0;
}

int Request::parseHeaders()
{
	size_t pos = _buffer.find("\r\n");

	while (pos != std::string::npos)
	{
		if (pos == 0)
		{
			_buffer.erase(0, 2);
			if (_headers.find("host") == _headers.end())
				return 400;
			if (_method == "POST" || _method == "PUT")
			{
				_status = PREBODY;
				return 0;
			}
			else
			{
				_status = DONE;
				return 0;
			}
		}
		size_t posColon = _buffer.find(":");
		if (posColon == std::string::npos)
			return 400;
		std::string key = _buffer.substr(0, posColon);
		if (hasWhiteSpaces(key))
			return 400;
		std::string value = _buffer.substr(posColon + 1, pos - posColon - 1);
		removeTrailingWhiteSpaces(value);
		toLowerCase(key);
		_headers[key] = value;
		_buffer.erase(0, pos + 2);
		pos = _buffer.find("\r\n");
	}
	return 0;
}

int Request::beforeParseBody()
{
	if (_headers.find("transfer-encoding") != _headers.end() 
		&& _headers["transfer-encoding"] == "chunked")
	{
		_status = CHUNKS;
		return createTempFile();
	}
	if (_headers.find("content-length") == _headers.end())
		return 411;
	else
	{
		try
		{
			_bodySize = std::stoi(_headers["content-length"]);
		}
		catch (const std::invalid_argument &e)
		{
			return 400;
		}
		catch (const std::out_of_range &e)
		{
			return 400;
		}
	}
	if (_bodySize == 0)
	{
		_status = DONE;
		return 0;
	}
	createTempFile();
	_status = BODY;
	return 0;
}

int Request::parseBody()
{
	if (_bodyBuffer.length() + _buffer.length() < BODY_BUFFER_LENGTH)
	{
		_bodyBuffer += _buffer;
		_bytesRead += _buffer.length();
		_buffer.clear();
	}
	else
	{
		if (writeToFile())
			return 500;
		_bodyBuffer += _buffer;
		_bytesRead += _buffer.length();
		_buffer.clear();
	}
	if (_bytesRead == _bodySize)
	{
		if (writeToFile())
			return 500;
		_status = DONE;
		_buffer.clear();
		return 0;
	}
	else if (_bytesRead > _bodySize)
		return 400;
	return 0;
}

int Request::parseChunks()
{
	size_t pos = _buffer.find("\r\n");

	while (pos != std::string::npos || _chunkStatus == CHUNK_DATA)
	{
		if (_chunkStatus == CHUNK_SIZE)
		{
			_chunkSize = std::stoi(_buffer.substr(0, pos), 0, 16);
			if (_chunkSize == 0)
			{
				_status = DONE;
				if (!_bodyBuffer.empty() && writeToFile())
					return 500;
				_buffer.clear();
				return 0;
			}
			_buffer.erase(0, pos + 2);
			_chunkStatus = CHUNK_DATA;
		}
		if (_chunkStatus == CHUNK_DATA)
		{
			if (static_cast<int>(_buffer.length()) >= _chunkSize + 2)
			{
				_bodyBuffer += _buffer.substr(0, _chunkSize);
				_bytesRead += _chunkSize;
				_buffer.erase(0, _chunkSize + 2);
				_chunkStatus = CHUNK_SIZE;
				pos = _buffer.find("\r\n");
			}
			else
			{
				if (static_cast<int>(_buffer.length()) == _chunkSize + 1
				|| static_cast<int>(_buffer.length()) == _chunkSize)
					return 0;
				_bodyBuffer += _buffer;
				_bytesRead += _buffer.length();
				_chunkSize -= _buffer.length();
				_buffer.clear();
				return 0;
			}
			if (_bodyBuffer.length() > BODY_BUFFER_LENGTH)
			{
				if (writeToFile())
					return 500;
			}
		}
	}
	return 0;
}

int Request::parse(const std::string &requestChunk)
{
	_buffer += requestChunk;
	if (_status == REQUEST_LINE)
		_errorCode = parseRequestLine(); // parse the first line of the request
	if (_status == HEADERS)
		_errorCode = parseHeaders(); // parse the headers of the request
	if (_status == PREBODY)
		_errorCode = beforeParseBody(); // parse the body of the request
	if (_status == BODY)
		_errorCode = parseBody(); // parse the body of the request
	if (_status == CHUNKS)
		_errorCode = parseChunks(); // parse chunks of the body of the request
	return _errorCode;
}
