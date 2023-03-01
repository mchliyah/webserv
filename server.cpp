/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 08:44:52 by slahrach          #+#    #+#             */
/*   Updated: 2023/03/01 11:09:54 by slahrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

std::string process_request() {
    // Parse the request and generate a response
    // In this example, we always return a "Hello, world!" response
    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/plain\r\n";
    response += "Content-Length: 13\r\n";
    response += "\r\n";
    response += "Hello, world!";
    return response;
}
server::server(int port_) :listner(-1) , port(port_){}

void server::createBindListen()
{
	struct addrinfo hints, *res, *p;
	int	yes = 1;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(NULL, "3000", &hints, &res) != 0)
		throw std::runtime_error("getaddrinfo error");
	for(p = res; p != NULL; p = p->ai_next)
	{
		listner = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
		if (listner < 0)
			continue;
		setsockopt(listner, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		if (bind(listner, p->ai_addr, p->ai_addrlen) < 0) {
			close(listner);
			continue;
		}
		break;
	}

	if (p == NULL)
		throw std::runtime_error("cant bind it");
	freeaddrinfo(res);
	int flags = fcntl(listner, F_GETFL, 0);
	fcntl(listner, F_SETFL, flags | O_NONBLOCK);
	if (listen(listner, 10) == -1)
		throw std::runtime_error("listen");
	std::cout << "listening on port " << port << std::endl;
}

void server::start()
{
	createBindListen();
	v.reserve(250);
	while (1)
	{
		fd_set	read_fds;
		FD_ZERO(&read_fds);
		FD_SET(listner, &read_fds);
		int		maxSocket = listner;
		for (std::vector<int>::iterator b = v.begin(); b < v.end(); b++)
		{
			FD_SET(*b, &read_fds);
			if (*b > maxSocket)
				maxSocket = *b;
		}
		struct timeval timeout;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		int activity = select(maxSocket + 1, &read_fds, NULL, NULL, &timeout);
		if (activity == -1) {
			std::cerr << "Error in select\n";
			break;
		}
		else
		{
			if (FD_ISSET(listner, &read_fds))
			{
				struct sockaddr_storage addr;
				socklen_t len = sizeof(addr);
				int newSocket = accept(listner, reinterpret_cast<sockaddr*>(&addr), &len);
				if (newSocket == -1)
					throw std::runtime_error("accept");
				int flags = fcntl(newSocket, F_GETFL, 0);
				fcntl(newSocket, F_SETFL, flags | O_NONBLOCK);
				v.push_back(newSocket);
			}
			for (std::vector<int>::iterator b = v.begin(); b < v.end(); b++)
			{
				if (FD_ISSET(*b, &read_fds))
				{
					char	buf[1028];
					int	r = recv(*b, buf, sizeof(buf), 0);
					if (r <= 0)
					{
						close(*b);
						FD_CLR(*b, &read_fds);
						v.erase(b);
					}
					else
					{
						std::string response = process_request();
						send(*b, response.c_str(), response.size(), 0);
					}
				}
			}
		}
	}
}
server::~server()
{
	if (listner != -1)
		close(listner);
	for (std::vector<int>::iterator b = v.begin(); b < v.end(); b++)
		close(*b);
}