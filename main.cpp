/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 05:09:29 by slahrach          #+#    #+#             */
/*   Updated: 2023/03/03 21:11:04 by slahrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"
int main()
{
	try
	{
		// std::string s = "    GET     kkk      a     \r\nHost:n\r\nContent-Type:mm\r\n\r\nbodyyyy";
		// request req(s);
		// std::cout << req.getError() << "   " << req.getErrorMessage() << std::endl;
		// req.printAttr();
		std::vector<std::string> v;
		v.push_back("80");
		v.push_back("3000");
		v.push_back("9000");
		v.push_back("5000");
		server my(v);
		my.start();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}