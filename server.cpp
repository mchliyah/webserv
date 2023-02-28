/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 08:44:52 by slahrach          #+#    #+#             */
/*   Updated: 2023/02/28 09:22:00 by slahrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
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
		// lose the pesky "address already in use" error message
		setsockopt(listner, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
		if (bind(listner, p->ai_addr, p->ai_addrlen) < 0) {
			close(listner);
			continue;
		}
		break;
	}

    // if we got here, it means we didn't get bound
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
	char	buf[1028];
	const char *msg = "HTTP/1.1 200 OK\r\nContent-Type: text/plain\r\n\r\nHello, world!";
	int bytes_sent;
	fd_set	read_fds;
	fd_set	write_fds;
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
		FD_ZERO(&read_fds);
		FD_ZERO(&write_fds);
		for (std::vector<int>::iterator b = v.begin(); b < v.end(); b++)
		{
			FD_SET(*b, &read_fds);
			FD_SET(*b, &write_fds);
		}
		maxSocket = *std::max_element(v.begin(), v.end());
		select(maxSocket + 1, &read_fds, &write_fds, NULL, NULL);
		for (std::vector<int>::iterator b = v.begin(); b < v.end(); b++)
		{
			if (FD_ISSET(*b, &read_fds))
			{
				if (*b == listner)
				{
					len = sizeof(addr);
					newSocket = accept(listner, reinterpret_cast<sockaddr*>(&addr), &len);
					if (newSocket == -1)
						throw std::runtime_error("accept");
					int flags = fcntl(newSocket, F_GETFL, 0);
					fcntl(newSocket, F_SETFL, flags | O_NONBLOCK);
					v.push_back(newSocket);
				}
				else
				{
					if (fork() == 0)
					{
						int	r = recv(*b, buf, sizeof buf, 0);
						if (r <= 0)
						{
							close(*b);
							v.erase(b);
						}
						else
						{
							std::string s = buf;
							request r(s);
							r.print_attr();
							exit(0);
						}
					}
				}
			}
			else if (FD_ISSET(*b, &write_fds))
				bytes_sent = send(*b, msg, strlen(msg), 0);
		}
	}
}
server::~server()
{
	if (listner != -1)
		close(listner);
}