/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 05:09:29 by slahrach          #+#    #+#             */
/*   Updated: 2023/03/01 08:49:17 by slahrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
int main()
{
	try
	{
		server my(3000);
		my.start();
		// std::string	s = "POST /api/login HTTP/1.1\r\nHost: example.com\r\nContent-Type: application/json\r\nContent-Length: 42\r\n\r\nouioui";
		// request r(s);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}