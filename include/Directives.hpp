/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Directives.hpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 12:11:53 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/18 18:27:01 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DIRECTIVES_HPP
#define DIRECTIVES_HPP

#define SERVER_BLOCK "server "
#define LOCATION_BLOCK "location "
#define LISTEN "listen "
#define LSTN_LOC_LIMIT_SIGN 'l'
#define SERVER_NAME "server_name "
#define SERVER_NAME_SIGN 's'
#define AUTOINDEX "autoindex "
#define AUTOINDEX_SIGN 'a'
#define ROOT "root "
#define ROOT_RETURN_SIGN 'r'
#define INDEX "index "
#define INDEX_SIGN 'i'
#define LIMIT_EXCEPT "limit_except "
#define ERROR_PAGE "error_page "
#define ERROR_PAGE_SIGN 'e'
#define RETURN "return "
#define TEMP_PATH "client_body_temp_path "
#define BODY_SIZE "client_max_body_size "
#define TEMP_BODY_SIGN 'c'
#define END_DIRECTIVE_SIGN ';'

#endif
