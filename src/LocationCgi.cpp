/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationCgi.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 15:59:36 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/09 19:13:29 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationCgi.hpp"

LocationCgi::LocationCgi() : Location() {
}

LocationCgi::LocationCgi(const LocationCgi& other) : Location(other) {
}

LocationCgi::~LocationCgi() {
}
Location& LocationCgi::operator=(const LocationCgi& other) {

}

std::string LocationCgi::getCgiPass() {
	
}
