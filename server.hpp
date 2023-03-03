/* ************************************************************************** */
/*	                                                                    */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 08:44:49 by slahrach          #+#    #+#             */
/*   Updated: 2023/02/24 09:22:18 by slahrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
#include <iostream>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdexcept>
#include <sys/select.h>
#include <vector>
#include <algorithm>
#include <utility>
# include "client.hpp"

class server
{
	private:
		std::vector<std::string> ports;
		std::vector<std::pair<int, std::string> > listners;
		std::vector<client> clients;
	public:
		server(std::vector<std::string> ports_);
		void start();
		std::pair<int,std::string> createBindListen(std::string port);
		~server();
};
#endif
