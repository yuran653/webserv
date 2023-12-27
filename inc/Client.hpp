/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etetopat <etetopat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/14 18:57:45 by etetopat          #+#    #+#             */
/*   Updated: 2023/12/14 19:44:51 by etetopat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include "Webserv.hpp"
//# include HttpRequest header
//# include response header

class   Client {
	public:
		Client();
		//Client(ServerConfig& server);
		Client(Client const& src);
		Client& operator=(Client const& rhs);
		~Client();

		int const&			getSocket() const;
		sockaddr_in const&	getAddr() const;
		//HttpRequest const&	getRequest() const;
		time_t const&		getLastRequestTime() const;

		void				setSocket(int& socket);
		void				setAddr(sockaddr_in& addr);
		//void				setServer(ServerConfig& server);
		
		void				buildResponse();
		void				updateLastRequestTime();
		void				clearClient();

		//HttpRequest 	request;
		//Response		response;
		//ServerConfig	server;

		private:
			int			_clientSocket;
			sockaddr_in	_clientAddr;
			time_t		_lastRequestTime;
};

#endif