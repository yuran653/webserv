/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationCgi.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 15:58:55 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/09 19:07:24 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONCGI_HPP
#define LOCATIONCGI_HPP

#include "Location.hpp"

class LocationCgi : public Location {
	private:
		std::string	_cgi_pass;
	
	public:
		LocationCgi();
		LocationCgi(const LocationCgi& other);
		~LocationCgi();

		Location& operator=(const LocationCgi& other);

		std::string					getCgiPass();
};

#endif
