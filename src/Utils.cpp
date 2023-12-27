/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etetopat <etetopat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 22:24:53 by etetopat          #+#    #+#             */
/*   Updated: 2023/12/12 22:30:26 by etetopat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"

int stringToInt(std::string str) {
	std::stringstream ss(str);
	
	if (str.length() > 10)
		throw std::exception();
	for (size_t i = 0; i < str.length(); ++i)
		if (!isdigit(str[i]))
			throw std::exception();
	int res;
	ss >> res;
	return (res);
}

unsigned int	hexToDec(std::string const& nb) {
	std::stringstream	ss;
	unsigned int		x;

	ss << nb;
	ss >> std::hex >> x;
	return (x);
}

