/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchliyah <mchliyah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 17:08:17 by mchliyah          #+#    #+#             */
/*   Updated: 2023/04/08 05:32:39 by mchliyah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"


void check_all_set(std::vector<serverconfig>& servers)
{
	std::vector<serverconfig>::iterator it;
	std::map<std::string, locationconfig>::iterator it2;

	for (it = servers.begin(); it != servers.end(); it++)
	{
		if (it->getServerName().empty())
			it->setServerName("localhost");
		if (it->getListen().empty())
			it->setListen("80");
		if (it->getLocations().empty() || it->getLocations().find("/") == it->getLocations().end())
		{
			locationconfig loc;
			loc.setRoot(std::getenv("PWD"));
			loc.setName("/");
			std::map<std::string, bool> m;
			m.insert(std::pair<std::string, bool>("GET", true));
			m.insert(std::pair<std::string, bool>("POST", false));
			m.insert(std::pair<std::string, bool>("DELETE", false));
			loc.setAllowsMethod(m);
			loc.setCgiPass("off");
			loc.getIndex().insert(loc.getIndex().begin(), "index.html");
			loc.setAutoIndex("off");
			it->getLocations().insert(std::pair<std::string, locationconfig>("/", loc));
		}
		for (it2 = it->getLocations().begin(); it2 != it->getLocations().end(); it2++)
		{
			if (it2->second.getRoot().empty())
				it2->second.setRoot(std::getenv("PWD"));
			if (it2->second.getAllowsMethod().empty())
			{
				std::map<std::string, bool> m;
				m.insert(std::pair<std::string, bool>("GET", true));
				m.insert(std::pair<std::string, bool>("POST", false));
				m.insert(std::pair<std::string, bool>("DELETE", false));
				it2->second.setAllowsMethod(m);
			}
			if (it2->second.getCgiPass().empty())
				it2->second.setCgiPass("off");
			if (it2->second.getIndex().empty())
				it2->second.getIndex().insert(it2->second.getIndex().begin(), "index.html");
			if (it2->second.getAutoIndex().empty())
				it2->second.setAutoIndex("off");
		}
		// it->printServer();
	}
}

std::vector<serverconfig>& parse(std::vector<serverconfig>& servers, std::string path){
	std::string line;
    std::ifstream os(path);

	if (!os.is_open())
		throw std::runtime_error("Error: can't open config file");
	std::getline(os, line);
	g_tab_count = tab_count(line);
	while (!os.eof())
	{
		serverconfig server;
		if (line.empty())
			continue;
		if (line.find("#") != std::string::npos)
			throw std::runtime_error("Error: comment is not allowed");
		if (line.find("server") == std::string::npos || line.find("server") != 0)
			throw std::runtime_error("Error: ruller is not at the top of the file");
		line = server.readServer(os, line);
		servers.push_back(server);
	}
	check_all_set(servers);
	return (servers);
}