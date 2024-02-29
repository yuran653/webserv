/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Defines.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jgoldste <jgoldste@student.42bangkok.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/11 12:11:53 by jgoldste          #+#    #+#             */
/*   Updated: 2024/02/18 22:24:24 by jgoldste         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef DEFINES_HPP
#define DEFINES_HPP

#define DEFAULT "./.config/default.conf"

#define EOT '\x04' // end of transmission
#define EOF_STR "\0xEOF"
#define COMMENT_SIGN '#'
#define SPACE_SIGN ' '
#define NEW_LINE_SIGN '\n'
#define BLOCK_OPEN_SIGN '{'
#define BLOCK_CLOSE_SIGN '}'
#define WILDCARD_SIGN '*'
#define WILDCARD_PATH "/*."
#define QUESTION_SIGN '?'

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
#define CGI_PASS "cgi_pass "
#define TEMP_PATH "client_body_temp_path "
#define BODY_SIZE "client_max_body_size "
#define CGI_TEMP_BODY_SIGN 'c'
#define END_DIRECTIVE_SIGN ';'

#define LISTEN_DELIM ':'
// #define DEFAULT_SERVER " default_server"
// #define DEFAULT_SERVER_SIGN "_"
#define LOCAL_HOST_NAME "localhost"
#define LOCAL_HOST_ADDR "127.0.0.1"
#define ROOT_LOCATION "/"
#define SLASH_SIGN '/'
#define HOST_DELIM '.'
#define DOT '.'

#define HTTP_STR "http://"
#define HTTPS_STR "https://"
#define HTTP_SIGN 'h'

#define ON "on"
#define OFF "off"

#define MIN_ERROR_PAGE_CODE 400
#define MAX_ERROR_PAGE_CODE 511
#define MIN_CODE 200
#define MAX_CODE 599

#define KB 'K'
#define MB 'M'
#define GB 'G'
#define MAX_SIZE_GB 10

#define RED "\033[1;31m"
#define RESET_RED "\033[0m"

#define BUFFSIZE 5000
#define PORT 8080

#define MAX_PATH_LENGTH 2048
#define MAX_CHUNK_SIZE 65536
#define BODY_BUFFER_LENGTH 100000
#define MAX_FILE_SIZE_FOR_STRING 10000

#define MAX_SOCK_CONNECTIONS 132

#define MAX_HTTP_HDR 4096
#define BUFF_SIZE 512
#define DBL_CRLF "\r\n\r\n"

#endif
