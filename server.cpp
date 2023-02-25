/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 08:44:52 by slahrach          #+#    #+#             */
/*   Updated: 2023/02/25 07:56:36 by slahrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
server::server(int port_) :listner(-1) , port(port_){}

void server::createBindListen()
{
	struct addrinfo hints, *res;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	//const char *string = std::to_string(port);
	if (getaddrinfo(NULL, "3000", &hints, &res) != 0)
		throw std::runtime_error("getaddrinfo error");
	if((listner = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) == -1)
			throw std::runtime_error("cant socket");
	if (fcntl(listner, F_SETFL, O_NONBLOCK) == -1)
		throw std::runtime_error("cant make it non blocking");
	// Bind the socket to the localhost address and port 8080
	if (bind(listner, res->ai_addr, res->ai_addrlen))
		throw std::runtime_error("cant bind it");

	//freeaddrinfo(res);
	if (listen(listner, 10) == -1)
		throw std::runtime_error("listen");
	std::cout << "listening on port " << port << std::endl;
}

void server::start()
{
	fd_set	read_fds;
	int		newSocket;
	int		maxSocket;
	std::vector<int> v;
	struct sockaddr_storage addr;
	socklen_t len;

	createBindListen();
	v.push_back(listner);
	v.reserve(250);
	while (1)
	{
		std::cout << "iteration" << std::endl;
		FD_ZERO(&read_fds);
		for (std::vector<int>::iterator b = v.begin(); b < v.end(); b++)
		{
			std::cout << "ouii " << *b << std::endl;
			FD_SET(*b, &read_fds);
		}
		maxSocket = *std::max_element(v.begin(), v.end());
		std::cout <<" maxSocket : " << maxSocket << std::endl;
		select(maxSocket + 1, &read_fds, NULL, NULL, NULL);
		for (std::vector<int>::iterator b = v.begin(); b < v.end(); b++)
		{
			if (FD_ISSET(*b, &read_fds))
			{
				if (*b == listner)
				{
					std::cout << "new connection " <<std::endl;
					//add new connection?
					//accept it check if it's bigger than max and add it to sockets so that select can detect it after
					len = sizeof(addr);
					newSocket = accept(listner, reinterpret_cast<sockaddr*>(&addr), &len);
					if (newSocket == -1)
						throw std::runtime_error("accept");
					v.push_back(newSocket);
					std::cout <<  newSocket <<" pushed " << std::endl;
				}
				else
				{
					std::cout << *b << " is readyyy " << std::endl;
					//char	buf[512];
					const char *msg = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, world!";
					int bytes_sent;
					//recv(*b, buf, sizeof buf, 0);
					//std::cout << "received  " << std::endl;
					bytes_sent = send(*b, msg, strlen(msg), 0);
					std::cout << bytes_sent << "sent successfully" << std::endl;
					
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