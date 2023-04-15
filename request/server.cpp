/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <mchliyah@student.1337.ma>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 08:44:52 by slahrach          #+#    #+#             */
/*   Updated: 2023/04/14 03:13:42 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

server::server(std::vector<std::pair<std::string, std::string> >ph, std::vector<serverconfig>& servers) : ports_hosts(ph), hosts(servers) {}

std::pair<int,std::pair<std::string, std::string> > server::createBindListen(std::pair<std::string, std::string>&ph)
{
	int yes = 1;
	int port = atoi(ph.first.c_str());
	int listner = socket(AF_INET, SOCK_STREAM, 0);
	if (listner == -1)
		throw std::runtime_error("webserver (socket)");
	struct sockaddr_in host_addr;
	//memset(&host_addr, 0, sizeof(host_addr));
	host_addr.sin_family = AF_INET;
	host_addr.sin_addr.s_addr = inet_addr(ph.second.c_str());
	host_addr.sin_port = htons(port);
	setsockopt(listner, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));
	if (bind(listner, (struct sockaddr *)&host_addr, sizeof(host_addr)) != 0)
		throw std::runtime_error("webserver (bind)");
	fcntl(listner, F_SETFL, O_NONBLOCK);
	if (listen(listner, 128) != 0)
		throw std::runtime_error("webserver (listen)");
	std::cout << "listening on " << ph.second << ":" <<port << std::endl;
	std::pair<int, std::pair<std::string, std::string> > r(listner, ph);
	return (r);
}

void server::start()
{
	clients.reserve(300);
	for (std::vector<std::pair<std::string, std::string> >::iterator p = ports_hosts.begin(); p < ports_hosts.end(); p++)
		listners.push_back(createBindListen(*p));
	while (1)
	{
		signal(SIGPIPE, SIG_IGN);
		fd_set read_fds;
		fd_set write_fds;
		FD_ZERO(&read_fds);
		FD_ZERO(&write_fds);
		for (std::vector<std::pair<int, std::pair<std::string, std::string> > >::iterator l = listners.begin(); l < listners.end(); l++)
			FD_SET(l->first, &read_fds);
		int maxSocket = std::max_element(listners.begin(), listners.end())->first;
		for (std::vector<client>::iterator c = clients.begin(); c < clients.end(); c++)
		{
			if (c->rcv < 4 && c->rcv >= 0)
				FD_SET(c->getSocket(), &read_fds);
			if (c->rcv==4)
				FD_SET(c->getSocket(), &write_fds);
			if (c->getSocket() > maxSocket)
				maxSocket = c->getSocket();
		}
		struct timeval timeout;
		timeout.tv_sec = 2;
		timeout.tv_usec = 0;
		int activity = select(maxSocket + 1, &read_fds, &write_fds, NULL, &timeout);
		if (activity == -1)
		{
			std::cout << "select error" << std::endl;
			continue;
		}
		for (std::vector<std::pair<int, std::pair<std::string, std::string> > >::iterator listner = listners.begin(); listner < listners.end(); listner++)
		{
			if (FD_ISSET(listner->first, &read_fds))
			{
				struct sockaddr_storage addr;
				socklen_t len = sizeof(addr);
				int newSocket = accept(listner->first, reinterpret_cast<sockaddr *>(&addr), &len);
				if (newSocket == -1)
				{
					std::cout << "accept failed" << std::endl;
					continue;
				}
				std::cout << "new connection on port " << listner->second.second << ":" <<listner->second.first << " : " << newSocket <<  std::endl;
				fcntl(newSocket, F_SETFL, O_NONBLOCK);
				client c(newSocket, listner->second);
				clients.push_back(c);
			}
		}
		for (std::vector<client>::iterator c = clients.begin(); c < clients.end(); c++)
		{
			if ((activity == 0 && c->rcv > 0 && c->rcv < 4))
			{
				std::cout << "timout"<< std::endl;
				c->rcv = 4;
				c->setError(408);
			}
			if (FD_ISSET(c->getSocket(), &read_fds))
			{
				char buf[1000000];
				int	r = recv(c->getSocket(), buf, 1000000, 0);
				c->last_rcv = std::clock();
				if (r <= 0)
				{
					std::cout << "closing socket" << c->getSocket() << std::endl;
					close(c->getSocket());
					FD_CLR(c->getSocket(), &read_fds);
					FD_CLR(c->getSocket(), &write_fds);
					clients.erase(c);
					break ;
				}
				else if (c->rcv != 4)
				{
					std::string buff(buf, buf + r);
					if (c->rcv == 0)
						c->addToRequestCheck(buff);
					else
					{
						if (c->rcv == 1)
							c->rcv = 2;
						if (c->rcv != 4)
							c->addToBody(buff);
					}
				}
			}
			if (FD_ISSET(c->getSocket(), &write_fds))
			{
				std::stringstream stream;
				int error = 0;
				if (c->getFirstTime()) 
				{
					c->matchHost(this->hosts);
					c->checkBodySize();
					stream << c->getError();
					c->setRes(response());
					if (stream.str() != "200")
					{
						error = 1;
						c->getRes().set_status_code(stream.str());
						c->errorResponse(c->getRes());
						c->setBuff(c->getRes().get_header() + c->getRes().get_body());
					}
				}
				int toSend = 0;
				if (!error)
				{
					switch (c->getValue("Method")[0])
					{
						case 'G':
							c->getRes().get_response(*c);
							break;
						case 'P':
							c->getRes().post_response(*c);
							break;
						case 'D':
							c->getRes().delete_response(*c);
					}
				}
				toSend = c->getSentBytes();
				while (toSend > 0)
				{
					int bytes = send(c->getSocket(), c->getBuff().c_str(), toSend, 0);
					c->snd += bytes;
					toSend -= bytes;
					if (bytes == -1)
					{
						std::cout << "closing socket" << c->getSocket() << std::endl;
						close(c->getSocket());
						FD_CLR(c->getSocket(), &read_fds);
						FD_CLR(c->getSocket(), &write_fds);
						clients.erase(c);
						break;
					}
				}
				if (c->getIsSent() == 1)
				{
					c->removeFiles();
					if (c->getValue("Connection") == "keep-alive")
						c->resetClient();
					else
					{
						close(c->getSocket());
						FD_CLR(c->getSocket(), &read_fds);
						FD_CLR(c->getSocket(), &write_fds);
						clients.erase(c);
					}
					break ;
				}
				c->setSentBytes(0);
				c->getRes().clear();
			}
		}
	}
}

server::~server(){}