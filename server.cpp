/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 08:44:52 by slahrach          #+#    #+#             */
/*   Updated: 2023/02/24 10:40:31 by slahrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
server::server(int port_) : port(port_), listner(-1) {}

server::createBindListen()
{
	struct addrinfo hints, *ai, *p;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(NULL, port, &hints, &ai) != 0)
		throw std::runtime_error("getaddrinfo error");
	for(p = ai; p != NULL; p = p->ai_next)
	{
		listener = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (listener < 0)
			continue;
		if (bind(listener, p->ai_addr, p->ai_addrlen) < 0){
			close(listener);
			continue;
		}
		break;
	}

	if (p == NULL)
		std::runtime_error("failed to bind");
	freeaddrinfo(ai);
	if (listen(listener, 10) == -1)
		throw std::runtime_error("listen");
}


server::~server()
{
	if (listner != -1)
		close(listner);
}