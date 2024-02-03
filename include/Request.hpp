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
#include <unistd.h>

#include "CodesTypes.hpp"

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

#define MAX_PATH_LENGTH 2048
#define MAX_CHUNK_SIZE 65536
#define FILE_PATH "file.txt"
#define BODY_BUFFER_LENGTH 10

class Request {
	private:
		std::string _method;
		std::string _path;
		std::string _version;
		std::string _query;
		std::map<std::string, std::string> _headers;
		int _fileFd;
		ssize_t _bytesRead;
		bool _parsingComplete;
		bool _readComplete;
		Status _status;
		std::string _bodyBuffer;
		std::string _buffer;
		ChunkStatus _chunkStatus;
		size_t _chunkSize;
		ssize_t _bodySize;
		int _errorCode;
	
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
		int getFileFd() const;
		int writeToFile();
		ssize_t getBytesRead() const;
		Status getStatus() const;
		bool isParsingComplete() const;
		void setReadComplete(bool readComplete);
		void setPath(const std::string &path);
		void setStatus(Status status);
		int	getErrorCode() const;
		int createTempFile();
		const std::map<std::string, std::string> &getHeaders() const;
};

std::ostream &operator<<(std::ostream &os, const Request &request);

#endif