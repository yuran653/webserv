/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etetopat <etetopat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 22:38:28 by etetopat          #+#    #+#             */
/*   Updated: 2023/12/14 22:11:11 by etetopat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ServerManager.hpp"

/* ------------------------------- CONSTRUCTOR -------------------------------*/

ServerManager::ServerManager() {}

/* -------------------------------- DESTRUCTOR --------------------------------*/

ServerManager::~ServerManager() {}

/* --------------------------------- METHODS ----------------------------------*/

// void	ServerManager::setupServers(std::vector<ServerConfig> servers) {
// 	1. Logging initialization message
// 	2. Assigning Server Configurations
// 	3. Iterating Over Server Configurations
// 	4. Checking for Duplicate Servers
// 	5. Handling Duplicate Servers:
// 	6. Server Setup for Unique Configurations
// 	7. Logging Server Creation
// 	8. Address Conversion for Logging
// }

void	ServerManager::runServers() {
	fd_set	readFdSetTmp;
	fd_set	writeSetTmp;
	int		selectRet;

	_maxFd = 0;
	initFdSets();
	struct timeval	timer;

	while (true) {
		timer.tv_sec = 1;
		timer.tv_usec = 0;
		readFdSetTmp = _readFdSet;
		writeSetTmp = _writeSet;

		if ((selectRet = select(_maxFd + 1, &readFdSetTmp, &writeSetTmp, NULL, &timer)) < 0) {
			// Log error
			exit(1);
			continue ;
		}
		for (int fd = 0; fd <= _maxFd; fd++) {
			if (FD_ISSET(fd, &readFdSetTmp) && _serversMap.count(fd))
				acceptNewConnection(_serversMap.find(fd)->second);
			else if (FD_ISSET(fd, &readFdSetTmp) && _clientsMap.count(fd))
				readRequest(fd, _clientsMap.find(fd)->second);
			else if (FD_ISSET(fd, &writeSetTmp) && _clientsMap.count(fd)) {
				// Handle CGI response
			}
		}
		timeoutCheck();
	}
}