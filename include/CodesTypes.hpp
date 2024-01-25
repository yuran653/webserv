/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   CodesTypes.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/24 18:54:05 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/25 21:49:03 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CODESTYPES_HPP
#define CODESTYPES_HPP

#include "Libraries.hpp"

class CodesTypes {
	private:
		CodesTypes();
		~CodesTypes();

		static std::map<int, std::string>			_initCodeMessages();
		static std::map<std::string, std::string>	_initMIMEType();
	
	public:
		static const std::map<int, std::string>			codeMessages;
		static const std::map<std::string, std::string>	MIMEType;
};

#endif
