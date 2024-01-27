/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CodesTypes.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 18:54:05 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/27 16:46:45 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODESTYPES_HPP
#define CODESTYPES_HPP

#include <map>
#include <string>
#include <vector>

class CodesTypes {
	private:
		CodesTypes();
		~CodesTypes();

		static std::vector<std::string>				_initHTTPmethods();
		static std::map<int, std::string>			_initCodeMessages();
		static std::map<std::string, std::string>	_initMIMEType();
	
	public:
		static const std::vector<std::string>			HTTPmethods;
		static const std::map<int, std::string>			codeMessages;
		static const std::map<std::string, std::string>	MIMEType;
};

#endif
