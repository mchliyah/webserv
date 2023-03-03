/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 05:09:29 by slahrach          #+#    #+#             */
/*   Updated: 2023/03/02 01:33:42 by slahrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
int main()
{
	try
	{
		std::string s = "    GET     kkk      a     \r\nHost:n\r\nContent-Type:mm\r\n\r\nbodyyyy";
		request req(s);
		std::cout << req.getError() << "   " << req.getErrorMessage() << std::endl;
		req.printAttr();
		// server my("80");
		// my.start();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}