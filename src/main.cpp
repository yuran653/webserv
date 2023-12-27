/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: etetopat <etetopat@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/12/12 22:36:20 by etetopat          #+#    #+#             */
/*   Updated: 2023/12/12 23:02:04 by etetopat         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Webserv.hpp"
#include "ServerManager.hpp"

// SIGPIPE handler
void    sigpipeHandle(int sig) { if (sig){} }

int main (int ac, char** av) {
    if (ac == 1 || ac == 2) {
        try {
            std::string config;
            // create object of ConfigParser
            // create object of ServerManager
            signal(SIGPIPE, sigpipeHandle); // handle SIGPIPE

            config = (ac == 1 ? "config/default.conf" : av[1]); // get config file path
            // run config parser
            // run server manager
        } catch (std::exception &e) {
            std::cerr << e.what() << std::endl;
            return (1);
        }
    } else {
        // Log error
        return (1);
    }
    return (0);
}
