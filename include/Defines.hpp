/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 12:11:53 by jgoldste          #+#    #+#             */
/*   Updated: 2024/01/29 16:34:29 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_HPP
#define DEFINES_HPP

#define DEFAULT "default.conf"
#define CGI_PATH "./cgi-bin/python"

#define EOT '\x04' // end of transmission
#define COMMENT_SIGN '#'
#define SPACE_SIGN ' '
#define NEW_LINE_SIGN '\n'
#define BLOCK_OPEN_SIGN '{'
#define BLOCK_CLOSE_SIGN '}'

#define SERVER_BLOCK "server "
#define LISTEN "listen "
#define LOCATION_BLOCK "location "
#define LIMIT_EXCEPT "limit_except "
#define LSTN_LOC_LIMIT_SIGN 'l'
#define SERVER_NAME "server_name "
#define SERVER_NAME_SIGN 's'
#define ERROR_PAGE "error_page "
#define ERROR_PAGE_SIGN 'e'
#define AUTOINDEX "autoindex "
#define AUTOINDEX_SIGN 'a'
#define ROOT "root "
#define RETURN "return "
#define ROOT_RETURN_SIGN 'r'
#define INDEX "index "
#define INDEX_SIGN 'i'
#define TEMP_PATH "client_body_temp_path "
#define BODY_SIZE "client_max_body_size "
#define TEMP_BODY_SIGN 'c'
#define END_DIRECTIVE_SIGN ';'

#define LISTEN_DELIM ':'
#define DEFAULT_SERVER " default_server"
#define DEFAULT_SERVER_SIGN "_"
#define LOCAL_HOST_NAME "localhost"
#define LOCAL_HOST_ADDR "127.0.0.1"
#define ROOT_LOCATION "/"
#define SLASH_SIGN '/'
#define HOST_DELIM '.'
#define DOT '.'

#define HTTP_STR "http://"
#define HTTPS_STR "https://"

#define ON "on"
#define OFF "off"

#define MIN_ERROR_PAGE_CODE 400
#define MAX_ERROR_PAGE_CODE 511

#endif
