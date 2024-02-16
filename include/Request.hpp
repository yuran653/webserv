#ifndef REQUEST_HPP
#define REQUEST_HPP

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <set>
#include <stack>
#include <regex>
#include <sstream>
#include <algorithm>
#include <fcntl.h>
#include <sys/stat.h>
#include <uuid/uuid.h>
#include <unistd.h>

#include "Config.hpp"
#include "CodesTypes.hpp"
#include "Defines.hpp"
#include "utils.hpp"

enum Status
{
	REQUEST_LINE,
	HEADERS,
	PREBODY,
	BODY,
	CHUNKS,
	DONE
};

enum ChunkStatus
{
	CHUNK_SIZE,
	CHUNK_DATA
};

class Request
{
private:
	std::string _method;
	std::string _path;
	std::string _version;
	std::string _query;
	std::map<std::string, std::string> _headers;
	int _tempFileFd;
	
	std::string _tempFilePath;
	int _chunkSize;
	ssize_t _bodySize;
	ssize_t _bytesRead;
	std::string _buffer;
	std::string _bodyBuffer;
	int _errorCode;
	Status _status;
	ChunkStatus _chunkStatus;

public:
	Request();
	~Request();
	Request(const Request &src);
	Request &operator=(const Request &src);

	const std::string &getMethod() const;
	const std::string &getPath() const;
	const std::string &getVersion() const;
	const std::string &getQuery() const;
	const std::string &getTempFilePath() const;
	int getTempFileFd() const;
	ssize_t getBytesRead() const;
	int getErrorCode() const;
	const std::map<std::string, std::string> &getHeaders() const;

	void setTempFileFd(int tempFileFd);

	int parse(const std::string &requestChunk);
	int parseRequestLine();
	int parseHeaders();
	int beforeParseBody();
	int parseBody();
	int parseChunks();
	void removeCurrentDirDots();
	void simplifyPath();
	int writeToFile();
	bool isParsingComplete() const;
	int createTempFile();
};

std::ostream &operator<<(std::ostream &os, const Request &request);

#endif