/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <mchliyah@student.1337.ma>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 05:09:29 by slahrach          #+#    #+#             */
/*   Updated: 2023/04/14 03:13:49 by codespace        ###   ########.fr       */
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
		std::vector<serverconfig> servers;
		if (argc != 2)
			throw std::runtime_error("Usage: ./webserv <config_file>");
		servers = parse(servers, argv[1]);
		std::vector<std::string> v;
		std::vector<serverconfig>::iterator it1;
		for (it1 = servers.begin(); it1 != servers.end(); it1++)
		{
			if (std::find_if(v.begin(), v.end(), compare(it1->getListen())) == v.end())
				v.push_back(it1->getListen());
		}
		server my(v, servers);
		srand(time(0));
		my.start();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
	return (0);
}