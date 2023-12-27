/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etetopat <etetopat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 22:39:23 by etetopat          #+#    #+#             */
/*   Updated: 2023/12/14 19:51:14 by etetopat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/* 
 * The Client class encapsulates all the functionalities needed to handle
 * an individual client's interaction with the server, 
 * including managing their requests, generating responses,
 * and maintaining connection-related information. 
 */

/* ------------------------------- CONSTRUCTORS -------------------------------*/

//Default constructor
Client::Client() : _lastRequestTime(0) {}

//Parameter constructor
// I need a method in HttpRequest to set max body size
// Client::Client(ServerConfig& server) {
//     setServer(server);
//     request.setMaxBodySize(server.getMaxBodySize());
//     _lastRequestTime = time(NULL);
// }

//Copy constructor
Client::Client(Client const& src) { *this = src; }

//Assignation operator overload
Client& Client::operator=(Client const& rhs) {
	if (this != &rhs) {
		this->_clientSocket = rhs._clientSocket;
		this->_clientAddr = rhs._clientAddr;
		//this->_request = rhs._request; 
		//this->_response = rhs._response;
		//this->_server = rhs._server;
		this->_lastRequestTime = rhs._lastRequestTime;
	}
	return *this;

/* -------------------------------- DESTRUCTOR --------------------------------*/

Client::~Client() {}

/* --------------------------------- GETTERS ----------------------------------*/

int const& Client::getSocket() const { return (this->_clientSocket); }

sockaddr_in const& Client::getAddr() const { return (this->_clientAddr); }

//HttpRequest const& Client::getRequest() const { return (this->_request); }

time_t const& Client::getLastRequestTime() const { return (this->_lastRequestTime); }

/* --------------------------------- SETTERS ----------------------------------*/

void Client::setSocket(int& socket) { this->_clientSocket = socket; }

void Client::setAddr(sockaddr_in& addr) { this->_clientAddr = addr; }

//void Client::setServer(ServerConfig& server) { this->_server = server; }

/* --------------------------------- METHODS ----------------------------------*/

// I need a method in Response.cpp to set request and build response
// void	Client::buildResponse() {
// 	response.setRequest(this->request);
// 	response.buildResponse();
// }

void	Client::updateLastRequestTime() { this->_lastRequestTime = time(NULL); }

// I need a method in Response.cpp to clear request
// void	Client::clearClient() {
// 	response.clear();
// 	request.clear();
// }