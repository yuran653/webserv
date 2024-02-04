/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CodesTypes.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 18:57:32 by jgoldste          #+#    #+#             */
/*   Updated: 2024/02/04 16:30:08 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "CodesTypes.hpp"

const std::set<std::string> CodesTypes::HTTPMethods = _initHTTPMethods();
const std::map<int, std::string> CodesTypes::codeMessages = _initCodeMessages();
const std::map<std::string, std::string> CodesTypes::MIMEType = _initMIMEType();

CodesTypes::CodesTypes() {
}

CodesTypes::~CodesTypes() {
}

std::set<std::string> CodesTypes::_initHTTPMethods() {
	std::set<std::string> return_value;
	return_value.insert("DELETE");
	return_value.insert("GET");
	return_value.insert("HEAD");
	return_value.insert("POST");
	return_value.insert("PUT");
	return return_value;
}

std::map<int, std::string> CodesTypes::_initCodeMessages() {
	std::map<int, std::string> return_value;
	return_value.insert(std::make_pair(200, "OK"));
	return_value.insert(std::make_pair(201, "Created"));
	return_value.insert(std::make_pair(202, "Accepted"));
	return_value.insert(std::make_pair(204, "No Content"));
	return_value.insert(std::make_pair(301, "Moved Permanently"));
	return_value.insert(std::make_pair(302, "Found"));
	return_value.insert(std::make_pair(400, "Bad Request"));
	return_value.insert(std::make_pair(401, "Unauthorized"));
	return_value.insert(std::make_pair(403, "Forbidden"));
	return_value.insert(std::make_pair(404, "Not Found"));
	return_value.insert(std::make_pair(405, "Method Not Allowed"));
	return_value.insert(std::make_pair(409, "Conflict"));
	return_value.insert(std::make_pair(411, "Length Required"));
	return_value.insert(std::make_pair(413, "Payload Too Large"));
	return_value.insert(std::make_pair(414, "URI Too Long"));
	return_value.insert(std::make_pair(500, "Internal Server Error"));
	return_value.insert(std::make_pair(501, "Not Implemented"));
	return_value.insert(std::make_pair(502, "Bad Gateway"));
	return_value.insert(std::make_pair(503, "Service Unavailable"));
	return_value.insert(std::make_pair(504, "Gateway Timeout"));
	return_value.insert(std::make_pair(505, "HTTP Version Not Supported"));
	return return_value;
}

std::map<std::string, std::string> CodesTypes::_initMIMEType() {
	std::map<std::string, std::string> return_value;
	return_value.insert(std::make_pair(".html", "text/html"));
	return_value.insert(std::make_pair(".css", "text/css"));
	return_value.insert(std::make_pair(".js", "application/javascript"));
	return_value.insert(std::make_pair(".png", "image/png"));
	return_value.insert(std::make_pair(".jpg", "image/jpeg"));
	return_value.insert(std::make_pair(".jpeg", "image/jpeg"));
	return_value.insert(std::make_pair(".gif", "image/gif"));
	return_value.insert(std::make_pair(".ico", "image/x-icon"));
	return_value.insert(std::make_pair(".svg", "image/svg+xml"));
	return_value.insert(std::make_pair(".pdf", "application/pdf"));
	return return_value;
}
