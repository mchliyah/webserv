/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/22 02:51:43 by slahrach          #+#    #+#             */
/*   Updated: 2023/02/24 10:12:37 by slahrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "socket.hpp"
#include <stdexcept>
#include <sys/socket.h>
#include <sys/types.h>

Socket_::Socket_(int sock) : Socket_fd(sock)
{
	if (sock < 0)
		throw std::runtime_error("socket error");
};

ssize_t Socket_::send(int s, const void *buf, size_t len, int flags) const
{
	return (std::send(s, buf, len, flags));
}

ssize_t Socket_::receive(char* buffer, size_t length) const
{
	return (std::recv(Socket_fd_, buffer, length, 0));
}

int Socket_::getSocket() const{return (Socket_fd);}