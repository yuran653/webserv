/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIInterface.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 12:40:44 by jgoldste          #+#    #+#             */
/*   Updated: 2024/02/19 19:25:24 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIInterface.hpp"

CGIInterface::CGIInterface() {
}

CGIInterface::~CGIInterface() {
}



int CGIInterface::_returnIfExeedsHeaderSize(char**& argv, std::string& header,
	char*& buff, int response_fd, const std::string& body_path) {
	header.clear();
	delete[] buff;
	close(response_fd);
	std::remove(body_path.c_str());
	return (_deleteServiceArgs(argv, 431));
}

size_t CGIInterface::_setBufSize() {
	size_t buff_size = BUFF_SIZE > sizeof(DBL_CRLF) - 1 ? BUFF_SIZE : sizeof(DBL_CRLF) - 1;
	buff_size = buff_size > MAX_HTTP_HDR - 1 ? MAX_HTTP_HDR : buff_size;
	return (buff_size);
}

std::string CGIInterface::_generateFileName(const int& length) {
	const std::string charset = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
	std::string filename;
	for (int i = 0; i < length; i++)
		filename += charset[rand() % (charset.size() + 1)];
	return filename;
}

void CGIInterface::_deleteCharArray(char**& array) {
	if (array != NULL) {
		for (size_t i = 0; array[i]; i++)
			delete[] array[i];
		delete[] array;
		array = NULL;
	}
}

int CGIInterface::_deleteServiceArgs(char**& argv, const int& status) {
	_deleteCharArray(argv);
	return status;
}

char** CGIInterface::_initArgv(const std::string& cgi_pass) {
	char** argv = new char*[2];
	argv[0] = new char[cgi_pass.size()];
	std::strcpy(argv[0], cgi_pass.c_str());
	argv[1] = NULL;
	return argv;
}

int CGIInterface::_execute(std::string& header, std::string& body_path,
		char**& envp, const std::string& cgi_pass, const int& file_fd) {
	int exit_status = 502;
	char** argv = _initArgv(cgi_pass);
	if (argv == NULL || envp == NULL)
		return _deleteServiceArgs(argv, exit_status);
	if (dup2(file_fd, STDIN_FILENO) == -1)
		return _deleteServiceArgs(argv, exit_status);
	int pipe_fd[2];
	if (pipe(pipe_fd) == -1)
		return _deleteServiceArgs(argv, exit_status);
	pid_t pid = fork();
	if (pid == -1)
		return _deleteServiceArgs(argv, exit_status);
	else if (pid == 0) {
		close(pipe_fd[0]);
		dup2(pipe_fd[1],STDOUT_FILENO);
		if (execve(argv[0], argv, envp) == -1) {
			exit(_deleteServiceArgs(argv, exit_status));
		}
	} else if (pid > 0) {
		close(pipe_fd[1]);
		body_path = "./" + _generateFileName(32);
		int response_fd = open(body_path.c_str(), O_WRONLY | O_CREAT | O_EXCL | O_APPEND, 0644);
		if (response_fd == -1)
			return (_deleteServiceArgs(argv, 500));
		if (fcntl(response_fd, F_SETFL, O_NONBLOCK) == -1) {
			close(response_fd);
			return (_deleteServiceArgs(argv, 413));
		}
		size_t buff_size = _setBufSize();
		char* buff = new char[buff_size + 1];
		std::memset(buff, '\0', buff_size + 1);
		size_t pos = std::string::npos;
		size_t read_size;
		while ((read_size = read(pipe_fd[0], buff, buff_size))) {
			if (header.size() > MAX_HTTP_HDR)
				return (_returnIfExeedsHeaderSize(argv, header, buff, response_fd, body_path));
			std::string buff_read(buff);
			header += buff_read;
			pos = header.find(DBL_CRLF);
			if (pos != std::string::npos) {
				header.erase(pos + sizeof(DBL_CRLF) - 1);
				if (header.size() > MAX_HTTP_HDR)
					return (_returnIfExeedsHeaderSize(argv, header, buff, response_fd, body_path));
				write(response_fd, buff + pos + sizeof(DBL_CRLF), read_size - header.size());
				std::memset(buff, '\0', buff_size + 1);
				break;
			}
			std::memset(buff, '\0', buff_size + 1);
		}
		while ((read_size = read(pipe_fd[0], buff, buff_size))) {
			write(response_fd, buff, read_size);
			std::memset(buff, '\0', buff_size + 1);
		}
		close(pipe_fd[0]);
		close(response_fd);
		delete[] buff;
		int status;
		while (waitpid(-1, &status, WUNTRACED) == -1)
			;
		if (WEXITSTATUS(status) != EXIT_FAILURE)// || WIFEXITED(status) == EXIT_SUCCESS) // <---
			exit_status = 200;
	}
	return _deleteServiceArgs(argv, exit_status);
}

int	CGIInterface::executeCGI(std::string& header, std::string& body_path, char**& envp,
			const std::string& cgi_pass, const std::string& body_temp_path) {
	srand(time(NULL));
	int code = -1;
	int file_fd;
	file_fd = open(cgi_pass.c_str(), O_RDONLY);
	if (file_fd == -1)
		return 502;
	close(file_fd);
	file_fd = open(body_temp_path.c_str(), O_RDONLY);
	if (file_fd == -1)
		return 500;
	if (fcntl(file_fd, F_SETFL, O_NONBLOCK) == -1) {
		close(file_fd);
		return 413;
	}
	code = _execute(header, body_path, envp, cgi_pass, file_fd);
	close(file_fd);
	return code;
}
