/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 05:09:29 by slahrach          #+#    #+#             */
/*   Updated: 2023/04/08 03:36:04 by slahrach         ###   ########.fr       */
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
 
int main(int argc, char **argv)
{
	try
	{
		if (argc != 2)
			throw std::runtime_error("Usage: ./webserv <config_file>");
		std::vector<serverconfig> servers;
		servers = parse(servers, argv[1]);
		std::vector<std::string> v;
		std::cout << std::endl << std::endl;
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