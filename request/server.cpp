/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 08:44:52 by slahrach          #+#    #+#             */
/*   Updated: 2023/03/23 03:11:08 by slahrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

server::server(std::vector<std::string>& ports_, std::vector<serverconfig>& servers) : ports(ports_), hosts(servers){}

std::pair<int, std::string> server::createBindListen(std::string port)
{
	struct addrinfo hints, *res, *p;
	int	yes = 1;
	int listner;
	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; //assign the adress of my localhost for me
	if (getaddrinfo(NULL, port.c_str(), &hints, &res) != 0)
		throw std::runtime_error("gai_strerror()");
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
	signal(SIGPIPE, SIG_IGN);
	std::vector<serverconfig>::iterator serv = servers.begin();
	clients.reserve(300);
	for (std::vector<std::string>::iterator p = ports.begin(); p < ports.end(); p++)
		listners.push_back(createBindListen(*p));
	while (1)
	{
		fd_set	read_fds;
		fd_set	write_fds;
		FD_ZERO(&read_fds);
		FD_ZERO(&write_fds);
		for (std::vector<std::pair<int, std::string> >::iterator l = listners.begin(); l < listners.end(); l++)
			FD_SET(l->first, &read_fds);
		int		maxSocket = std::max_element(listners.begin(), listners.end())->first;
		for (std::vector<client>::iterator c = clients.begin(); c < clients.end(); c++)// Add client sockets to read_fds and write_fds
		{
			FD_SET(c->getSocket(), &read_fds);
			FD_SET(c->getSocket(), &write_fds);
			if (c->getSocket() > maxSocket)
				maxSocket = c->getSocket();
		}
		struct timeval timeout;
		timeout.tv_sec = 1;
		timeout.tv_usec = 0;
		int activity = select(maxSocket + 1, &read_fds, &write_fds, NULL, NULL);
		if (activity == -1) {
		perror("select error");
			break;
		}
		for (std::vector<std::pair<int, std::string> >::iterator listner = listners.begin(); listner < listners.end(); listner++)
		{
			if (FD_ISSET(listner->first, &read_fds))
			{
				struct sockaddr_storage addr;
				socklen_t len = sizeof(addr);
				int newSocket = accept(listner->first, reinterpret_cast<sockaddr*>(&addr), &len);
				if (newSocket == -1){
					perror("accept failed");
					continue;
				}
				std::cout << "new connection on port " << listner->second << " : " << newSocket <<  std::endl;
				// clients.push_back(client(newSocket, listner->second));
				int flags = fcntl(newSocket, F_GETFL, 0);
				fcntl(newSocket, F_SETFL, flags | O_NONBLOCK);
				client c(newSocket, listner->second);
				clients.push_back(c);
			}
		}
		for (std::vector<client>::iterator c = clients.begin(); c < clients.end(); c++)
		{
			if (FD_ISSET(c->getSocket(), &read_fds))
			{
				char	buf[1028];
				int	r = recv(c->getSocket(), buf, sizeof(buf), 0);
				if (r <= 0)
				{
					close(c->getSocket());
					FD_CLR(c->getSocket(), &read_fds);
					FD_CLR(c->getSocket(), &write_fds);
					clients.erase(c);
				}
				else
				{
					c->setIsSent(0);
					c->setRequest(buf);
					c->parse();
					c->matchHost(this->hosts);
					//c->getHost().printServer();
					//c->printAttr();
				}
			}
			else if (FD_ISSET(c->getSocket(), &write_fds) && !c->getIsSent())
			{
				response res(c->getValue("Method"));
				std::string response;
				if (c->getValue("Method") == "GET")
					response = res.get_response(*serv, c->getValue("URL"));
				else if (c->getValue("Method") == "POST")
					response = res.post_response(*serv, c->getValue("Path"), c->getValue("Body"));
				else if (c->getValue("Method") == "DELETE")
					response = res.delete_response(*serv, c->getValue("Path"));
				int bytes = send(c->getSocket(), response.c_str(), response.size(), 0);
				c->setIsSent(1);
				(void)bytes;
			}
		}
	}
}

server::~server()
{
	for (std::vector<std::pair<int, std::string> >::iterator l = listners.begin(); l < listners.end(); l++)
		close(l->first);
	for (std::vector<client>::iterator c = clients.begin(); c < clients.end(); c++)
		close(c->getSocket());
}