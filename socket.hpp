/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 21:24:17 by slahrach          #+#    #+#             */
/*   Updated: 2023/03/03 11:40:04 by slahrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SOCKET_HPP
# define SOCKET_HPP
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdexcept>
#include <sys/select.h>
#include <vector>
#include <algorithm>

class Socket_
{
	private:
		int			socket_fd;
		std::string	port;
	public:
		Socket_(int sock, std::string port_);
		size_t sd(int s, const void *buf, size_t len, int flags)const;
		size_t receive(char* buffer, size_t length) const;
		int getSocket() const;
		std::string getPort() const;
};
#endif