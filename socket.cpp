/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 02:51:43 by slahrach          #+#    #+#             */
/*   Updated: 2023/02/24 09:51:07 by slahrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>
socket_::socket_(int sock) : socket_fd(sock)
{
	if (sock < 0)
		throw std::runtime_error("socket error");
};

ssize_t socket_::send(int s, const void *buf, size_t len, int flags) const
{
	return (std::send(s, buf, len, flags));
}

ssize_t socket_::receive(char* buffer, size_t length) const
{
	return (std::recv(socket_fd_, buffer, length, 0));
}

int socket_::getSocket() const{return (socket_fd);}