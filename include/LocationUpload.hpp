/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationUpload.hpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 15:58:26 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/09 19:00:24 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LOCATIONUPLOAD_HPP
#define LOCATIONUPLOAD_HPP

#include "Location.hpp"

class LocationUpload : public Location {
	private:
		std::string					_client_body_temp_path;
		size_t						_client_max_body_size;

	public:
		LocationUpload();
		LocationUpload(const LocationUpload& other);
		~LocationUpload();

		LocationUpload& operator=(const LocationUpload& other);

		std::string	getClientBodyTempPath();
		size_t		getClientMaxBodySize();
};

#endif
