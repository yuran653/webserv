#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <regex>
#include <sstream>
#include <algorithm>

enum Status
{
	REQUEST_LINE,
	HEADERS,
	PREBODY,
	BODY,
	CHUNKS,
	DONE
};

#define MAX_PATH_LENGTH 2048

class Request {
	private:
		std::string _method;
		std::string _path;
		std::string _version;
		std::string _query;
		std::map<std::string, std::string> _headers;
		std::string _body;
		std::string _buffer;
		Status _status;
		size_t _bodySize;
	
	public:
		Request();
		~Request();
		Request(const Request &src);
		Request &operator=(const Request &src);
		int parse(const std::string &requestChunk);
		int parseRequestLine();
		int parseHeaders();
		int beforeParseBody();
		int parseBody();
		int parseChunks();
		int removeFirstNewLines();
		const std::string &getMethod() const;
		const std::string &getPath() const;
		const std::string &getVersion() const;
		const std::string &getQuery() const;
		const std::string &getBody() const;
		const std::map<std::string, std::string> &getHeaders() const;
};

std::ostream &operator<<(std::ostream &os, const Request &request);

#endif