/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchliyah <mchliyah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 05:09:29 by slahrach          #+#    #+#             */
/*   Updated: 2023/04/01 21:59:12 by mchliyah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "includes/server.hpp"

int g_tab_count = 0;

struct compare
{
    std::string my_port;
    compare(std::string my): my_port(my) {}
 
    bool operator()(std::string s) {
        return (s == my_port);
    }
};
 
// int main()
// {
//     std::vector<int> v = { 4, 7, 5, 2, 6, 9 };
//     int key = 6;
 
//     if (std::find_if(v.begin(), v.end(), compare(key)) != v.end()) {
//         std::cout << "Element found";
//     }
//     else {
//         std::cout << "Element not found";
//     }
 
//     return 0;
// }
int main(int argc, char **argv)
{
	try
	{
		if (argc != 2)
			throw std::runtime_error("Usage: ./webserv <config_file>");
		std::vector<serverconfig> servers = parse(argv[1]);
		std::vector<std::string> v;
		// std::string s = "    GET     kkk      a     \r\nHost:n\r\nContent-Type:mm\r\n\r\nbodyyyy";
		// request req(s);
		// std::cout << req.getError() << "   " << req.getErrorMessage() << std::endl;
		// req.printAttr();
	// std::vector<serverconfig>::iterator it = servers.begin();
	// 	while (it != servers.end())
	// 	{
	// 		it->printServer();
	// 		std::cout << " ====================================== "<< std::endl;
	// 		it++;
	// 	}
		
		std::vector<serverconfig>::iterator it1;
		for (it1 = servers.begin(); it1 != servers.end(); it1++)
		{
			if (std::find_if(v.begin(), v.end(), compare(it1->getListen())) == v.end())
				v.push_back(it1->getListen());
		}
		server my(v, servers);
		my.start();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return (0);
}