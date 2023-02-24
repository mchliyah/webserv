/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 08:44:52 by slahrach          #+#    #+#             */
/*   Updated: 2023/02/24 10:42:30 by slahrach         ###   ########.fr       */
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

server::start()
{
	fd_set	sockets;
	fd_set	read_fds;
	int		newSocket;
	int		maxSocket;
	struct sockaddr_storage addr;
	socklen_t len;

	createBindListen();
	//make it non blocking
	fcntl(listner, F_SETFL, O_NONBLOCK);
	maxSocket = listner;
	FD_SET(listner, sockets);
	while (1)
	{
		read_fds = sockets;
		select(maxSocket + 1, &read_fds, NULL, NULL, NULL);
		for (int i = 0; i < maxSocket; i++)
		{
			if (FD_ISSET(i, read_fds))
			{
				if (i == listner)
				{
					//add new connection
					//accept it check if it's bigger than max and add it to sockets so that select can detect it after
					len = sizeof(addr);
					newSocket = accept(listner, reinterpret_cast<sockaddr*>(&addr), &len);
					if (newSocket == -1)
						throw std::runtime_error("accept");
					if (newSocket > maxSocket)
						maxSocket = newSocket;
					FD_SET(newSocket, sockets);
				}
				else
				{
					//receive and respond
					Socket_ client(i);
					//if client.receive is 0 :
					FD_CLR(i, sockets);
				}
			}
		}
	}
}
server::~server()
{
	if (listner != -1)
		close(listner);
}