/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 08:44:52 by slahrach          #+#    #+#             */
/*   Updated: 2023/03/03 13:18:20 by slahrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

std::string process_request() {
    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/plain\r\n";
    response += "Content-Length: 13\r\n";
    response += "\r\n";
    response += "Hello, world!";
    return response;
}
server::server(std::vector<std::string> ports_) : ports(ports_){}

std::pair<int, std::string> server::createBindListen(std::string port)
{
	struct addrinfo hints, *res, *p;
	int	yes = 1;
	int listner;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	if (getaddrinfo(NULL, port.c_str(), &hints, &res) != 0)
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
	std::pair<int, std::string> r(listner, port);
	return (r);
}

void server::start()
{
	for (std::vector<std::string>::iterator p = ports.begin(); p < ports.end(); p++)
		listners.push_back(createBindListen(*p));
	while (1)
	{
		fd_set	read_fds;
		FD_ZERO(&read_fds);
		for (std::vector<std::pair<int, std::string> >::iterator l = listners.begin(); l < listners.end(); l++)
			FD_SET(l->first, &read_fds);
		int		maxSocket = std::max_element(listners.begin(), listners.end())->first;
		for (std::vector<Socket_>::iterator c = clients.begin(); c < clients.end(); c++)
		{
			FD_SET(c->getSocket(), &read_fds);
			if (c->getSocket() > maxSocket)
				maxSocket = c->getSocket();
		}
		struct timeval timeout;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		int activity = select(maxSocket + 1, &read_fds, NULL, NULL, &timeout);
		if (activity == -1) {
			throw std::runtime_error("select");
		}
		else
		{
			for (std::vector<std::pair<int, std::string> >::iterator listner = listners.begin(); listner < listners.end(); listner++)
			{
				if (FD_ISSET(listner->first, &read_fds))
				{
					struct sockaddr_storage addr;
					socklen_t len = sizeof(addr);
					int newSocket = accept(listner->first, reinterpret_cast<sockaddr*>(&addr), &len);
					if (newSocket == -1)
						throw std::runtime_error("accept");
					int flags = fcntl(newSocket, F_GETFL, 0);
					fcntl(newSocket, F_SETFL, flags | O_NONBLOCK);
					Socket_ client(newSocket, listner->second);
					clients.push_back(client);
				}
			}
			for (std::vector<Socket_>::iterator client = clients.begin(); client < clients.end(); client++)
			{
				if (FD_ISSET(client->getSocket(), &read_fds))
				{
					char	buf[1028];
					int	r = recv(client->getSocket(), buf, sizeof(buf), 0);
					if (r <= 0)
					{
						close(client->getSocket());
						FD_CLR(client->getSocket(), &read_fds);
						clients.erase(client);
					}
					else
					{
						std::string response = process_request();
						send(client->getSocket(), response.c_str(), response.size(), 0);
						
					}
				}
			}
		}
	}
}
server::~server()
{
	//close fds
}