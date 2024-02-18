/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CGIInterface.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/08 12:40:44 by jgoldste          #+#    #+#             */
/*   Updated: 2024/02/18 01:04:32 by jgoldste         ###   ########.fr       */
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

char** CGIInterface::_initEnv() {
	char** envp = new char*[4];
	envp[0] = new char[std::strlen("REQUEST_METHOD=POST") + 1];
	std::strcpy(envp[0], "REQUEST_METHOD=POST");
	envp[1] = new char[std::strlen("SERVER_PROTOCOL=HTTP/1.1") + 1];
	std::strcpy(envp[1], "SERVER_PROTOCOL=HTTP/1.1");
	envp[2] = new char[std::strlen("PATH_INFO=/directory/youpi.bla") + 1];
	std::strcpy(envp[2], "PATH_INFO=/directory/youpi.bla");
	envp[3] = NULL;
	return envp;
}

char** CGIInterface::_initArgv(const std::string& cgi_pass) {
	char** argv = new char*[2];
	argv[0] = new char[cgi_pass.size()];
	std::strcpy(argv[0], cgi_pass.c_str());
	argv[1] = NULL;
	return argv;
}

// int CGIInterface::_execute(std::pair<int, std::string>& response,
// 	const std::string& cgi_pass, const int& file_fd) {
// 	response.first = 502;
// 	char** argv = _initArgv(cgi_pass);
// 	char** envp = _initEnv();
// 	if (argv == NULL || envp == NULL)
// 		return _deleteServiceArgs(argv, envp, EXIT_FAILURE);
// 	if (dup2(file_fd, STDIN_FILENO) == -1)
// 		return _deleteServiceArgs(argv, envp, EXIT_FAILURE);
// 	pid_t pid = fork();
// 	if (pid == -1)
// 		return _deleteServiceArgs(argv, envp, EXIT_FAILURE);
// 	else if (pid == 0) {
// 		dup2(pipe_fd[1],STDOUT_FILENO);
// 		if (execve(argv[0], argv, envp) == -1) {
// 			close
// 			exit(_deleteServiceArgs(argv, envp, EXIT_FAILURE));
// 			}
// 	} else if (pid > 0) {
// 		close(pipe_fd[1]);
// 		char* buff = new char[BUFF_SIZE + 1];
// 		for (size_t i = 0; i <= BUFF_SIZE; i++)
// 			buff[i] = '\0';
// 		while (read(pipe_fd[0], buff, BUFF_SIZE)) {
// 			response.second += std::string(buff);
// 			for (size_t i = 0; i <= BUFF_SIZE; i++)
// 			buff[i] = '\0';
// 		}
// 		response.second += EOF_STR;
// 		int status;
// 		while (waitpid(-1, &status, WUNTRACED) == -1)
// 			;
// 		close(pipe_fd[0]);
// 		delete[] buff;
// 		if (WEXITSTATUS(status) != EXIT_FAILURE)
// 			response.first = 200;
// 	}
// 	return _deleteServiceArgs(argv, envp, EXIT_SUCCESS);
// }

// size_t CGIInterface::_setBufSize() {
// 	return (BUFF_SIZE > sizeof(DBL_CRLF) ? BUFF_SIZE : sizeof(DBL_CRLF));
// }

int CGIInterface::_execute(std::string& header, std::string& body_path,
		const std::string& cgi_pass, const int& file_fd) {
	body_path = "file.txt";
	int fd = open(body_path.c_str(), O_WRONLY | O_CREAT | O_TRUNC, 
		S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	std::ofstream outputFile(body_path);

	int exit_status = 502;
	char** argv = _initArgv(cgi_pass);
	char** envp = _initEnv();
	if (argv == NULL || envp == NULL)
		return _deleteServiceArgs(argv, envp, exit_status);
	size_t buff_size = BUFF_SIZE;
	if (dup2(file_fd, STDIN_FILENO) == -1)
		return _deleteServiceArgs(argv, envp, exit_status);
	int pipe_fd[2];
	if (pipe(pipe_fd) == -1)
		return _deleteServiceArgs(argv, envp, exit_status);
	pid_t pid = fork();
	if (pid == -1)
		return _deleteServiceArgs(argv, envp, exit_status);
	else if (pid == 0) {
		close(pipe_fd[0]);
		dup2(pipe_fd[1],STDOUT_FILENO);
		if (execve(argv[0], argv, envp) == -1)
			exit(_deleteServiceArgs(argv, envp, exit_status));
	} else if (pid > 0) {
		close(pipe_fd[1]);
		char* buff = new char[buff_size + 1];
		for (size_t i = 0; i <= buff_size; i++)
			buff[i] = '\0';
		while (read(pipe_fd[0], buff, buff_size)) {
			std::string buff_read(buff);
			header += buff_read;
			size_t pos = header.find(DBL_CRLF);
			if (pos != std::string::npos) {
				write(fd, buff + pos + sizeof(DBL_CRLF), pos + sizeof(DBL_CRLF));
				header.erase(pos + sizeof(DBL_CRLF));
				break;
			}
			for (size_t i = 0; i <= buff_size; i++)
				buff[i] = '\0';
		}
		while ((read(pipe_fd[0], buff, buff_size))) {
            outputFile << buff;
        }
		close(pipe_fd[0]);
		delete[] buff;
		int status;
		while (waitpid(-1, &status, WUNTRACED) == -1)
			;
		if (WEXITSTATUS(status) != EXIT_FAILURE) // || WIFEXITED(status) == EXIT_SUCCESS) // <---
			exit_status = 200;
	}
	(void)body_path;
	return _deleteServiceArgs(argv, envp, exit_status);
}

int	CGIInterface::executeCGI(std::string& header, std::string& body_path,
			const std::string& cgi_pass, const std::string& body_temp_path) {
	int code;
	int file_fd;
	file_fd = open(cgi_pass.c_str(), O_RDONLY);
	if (file_fd == -1)
		return 502;
	close(file_fd);
	file_fd = open(body_temp_path.c_str(), O_RDONLY);
	if (file_fd == -1)
		return 500;
	code = _execute(header, body_path, cgi_pass, file_fd);
	close(file_fd);
	return code;
}
