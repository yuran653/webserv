/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   LocationUpload.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/09 19:00:26 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/09 19:11:53 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "LocationUpload.hpp"

LocationUpload::LocationUpload() : Location() {
}

LocationUpload::LocationUpload(const LocationUpload& other) : Location(other) {
}

LocationUpload::~LocationUpload() {
}

LocationUpload& LocationUpload::operator=(const LocationUpload& other) {
}

std::string LocationUpload::getClientBodyTempPath() {
}

size_t LocationUpload::getClientMaxBodySize() {
}
