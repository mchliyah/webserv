/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchliyah <mchliyah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 05:09:29 by slahrach          #+#    #+#             */
/*   Updated: 2023/03/11 19:25:12 by mchliyah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

int g_tab_count = 0;

int main(int argc, char **argv)
{
	try
	{
		// std::string s = "    GET     kkk      a     \r\nHost:n\r\nContent-Type:mm\r\n\r\nbodyyyy";
		// request req(s);
		// std::cout << req.getError() << "   " << req.getErrorMessage() << std::endl;
		// req.printAttr();
		parse(argv[1]);
		// std::vector<std::string> v;
		// v.push_back("80");
		// v.push_back("3000");
		// v.push_back("9000");
		// v.push_back("5000");
		// server my(v);
		// my.start();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}