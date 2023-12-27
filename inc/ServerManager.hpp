/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ServerManager.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etetopat <etetopat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 22:38:39 by etetopat          #+#    #+#             */
/*   Updated: 2023/12/14 22:08:31 by etetopat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVERMANAGER_HPP
# define SERVERMANAGER_HPP

# include "Webserv.hpp"
# include "Client.hpp"
//# include "Response.hpp"

class ServerManager {
	public:
		ServerManager();
		ServerManager(ServerManager const& src);
		ServerManager & operator=(ServerManager const& rhs);
		~ServerManager();

		//void    setupServers(std::vector<ServerConfig>);
		void    runServers();

	private:
		// std::vector<ServerConfig>	_servers;
		// std::map<int, ServerConfig> _serversMap;
		std::map<int, Client>		_clientsMap;
		fd_set						_readFdSet;
		fd_set						_writeSet;
		int							_maxFd;

		void	initFdSets();
		//void	acceptNewConnection(ServerConfig& server);
		void	readRequest(int const& fd, Client& client);
		void	assignServer(Client& client);
		void	handleReqBody(Client& client);
		void	addToSet(int const& fd, fd_set& set);
		void	removeFromSet(int const& fd, fd_set& set);
		void	closeConnection(int const& fd);
		//void	sendCgiBody(Client& client), CgiHandler& cgiHandler); //I need a CgiHandler class
		//void	readCgiResponse(Client& client, CgiHandler& cgiHandler); //I need a CgiHandler class
		void	sendResponse(int const& fd, Client& client);
		void	timeoutCheck();
};

#endif