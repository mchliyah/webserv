/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   socket.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/23 21:24:17 by slahrach          #+#    #+#             */
/*   Updated: 2023/02/24 10:12:13 by slahrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

class Socket_
{
	private:
		int	socket_fd;
	public:
		Socket_(int sock);
		ssize_t send(int s, const void *buf, size_t len, int flags)const;
		ssize_t receive(char* buffer, size_t length) const;
		int getSocket() const;
};