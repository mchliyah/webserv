#include "server.hpp"


void parse(std::string path){
	std::string line;
    std::ifstream os(path);
	std::vector<serverconfig> servers;

	if (!os.is_open())
		throw std::runtime_error("Error: can't open config file");
	std::getline(os, line);
	g_tab_count = tab_count(line);
	while (!os.eof())
	{
		serverconfig server;
		if (line.find("#") != std::string::npos)
			throw std::runtime_error("Error: comment is not allowed");
		if (line.find("server") == std::string::npos || line.find("server") != 0)
			throw std::runtime_error("Error: ruller is not at the top of the file");
		line = server.readServer(os, line);
		// server.printServer();
		// std::cout << " ====================================== "<< std::endl;
		servers.push_back(server);
	}
	std::vector<serverconfig>::iterator it = servers.begin();
	while (it != servers.end())
	{
		it->printServer();
		std::cout << " ====================================== "<< std::endl;
		it++;
	}
}