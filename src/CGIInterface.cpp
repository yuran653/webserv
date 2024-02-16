/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIInterface.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 12:40:44 by jgoldste          #+#    #+#             */
/*   Updated: 2024/02/15 13:52:37 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CGIInterface.hpp"

CGIInterface::CGIInterface() {
}

CGIInterface::~CGIInterface() {
}

void CGIInterface::_deleteCharArray(char**& array) {
	if (array != NULL) {
		for (size_t i = 0; array[i]; i++)
			delete[] array[i];
		delete[] array;
		array = NULL;
	}
}

int CGIInterface::_deleteServiceArgs(char**& argv, char**& envp, const int& status) {
	_deleteCharArray(argv);
	_deleteCharArray(envp);
	return status;
}

char** CGIInterface::_initArgv(const std::string& cgi_pass) {
	char** argv = new char*[2];
	argv[0] = new char[cgi_pass.size() + 1];
	std::strcpy(argv[0], cgi_pass.c_str());
	argv[1] = NULL;
	return argv;
}

int CGIInterface::_execute(std::pair<int, std::string>& response,
	char** envp, const std::string& cgi_pass, const int& file_fd) {
	response.first = 502;
	char** argv = _initArgv(cgi_pass);
	if (argv == NULL || envp == NULL)
		return _deleteServiceArgs(argv, envp, EXIT_FAILURE);
	if (dup2(file_fd, STDIN_FILENO) == -1)
		return _deleteServiceArgs(argv, envp, EXIT_FAILURE);
	int pipe_fd[2];
	if (pipe(pipe_fd) == -1)
		return _deleteServiceArgs(argv, envp, EXIT_FAILURE);
	pid_t pid = fork();
	if (pid == -1)
		return _deleteServiceArgs(argv, envp, EXIT_FAILURE);
	else if (pid == 0) {
		close(pipe_fd[0]);
		dup2(pipe_fd[1],STDOUT_FILENO);
		if (execve(argv[0], argv, envp) == -1)
			exit(_deleteServiceArgs(argv, envp, EXIT_FAILURE));
	} else if (pid > 0) {
		close(pipe_fd[1]);
		char* buff = new char[BUFF_SIZE + 1];
		for (size_t i = 0; i <= BUFF_SIZE; i++)
			buff[i] = '\0';
		while (read(pipe_fd[0], buff, BUFF_SIZE)) {
			response.second += std::string(buff);
			for (size_t i = 0; i <= BUFF_SIZE; i++)
				buff[i] = '\0';
		}
		response.second += EOF_STR;
		// std::cerr << "_BEGIN->" << response.second << "<-_END" << std::endl;
		int status;
		while (waitpid(-1, &status, WUNTRACED) == -1)
			;
		close(pipe_fd[0]);
		delete[] buff;
		if (WEXITSTATUS(status) != EXIT_FAILURE || WIFEXITED(status) == EXIT_SUCCESS) // <---
			response.first = 200;
	}
	return _deleteServiceArgs(argv, envp, EXIT_SUCCESS);
}

int CGIInterface::_openFile(const char* path, int& response_code, const int& code) {
	int file_fd = open(path, O_RDONLY);
	if (file_fd == -1)
		response_code = code;
	return file_fd;
}

void CGIInterface::executeCGI(std::pair<int, std::string>& response, char** envp,
			const std::string& cgi_pass, const std::string& body_temp_path) {
	int file_fd;
	file_fd = _openFile(cgi_pass.c_str(), response.first, 502);
	if (file_fd == -1)
		return;
	close(file_fd);
	file_fd = _openFile(body_temp_path.c_str(), response.first, 500);
	if (file_fd == -1)
		return;
	_execute(response, envp, cgi_pass, file_fd);
	close(file_fd);
}