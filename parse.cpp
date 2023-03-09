#include "server.hpp"

bool has_only_spaces(std::string str)
{
	std::string::iterator it = str.begin();
	while (it != str.end())
	{
		if (*it != ' ' && *it != '\t')
			return false;
		it++;
	}
	return true;
}

int tab_count(std::string str) {
	return std::count(str.begin(), str.end(), '\t');
}

void parse(std::string path)
{
    std::string line;
    std::ifstream os(path);
    std::vector<std::map<std::string, std::string> > herservers;
    std::map<std::string, std::string> current_server;
	int track = 0;

    bool server = false;
    bool location = false;

    if (!os.is_open())
        throw std::runtime_error("Error: can't open config file");
	getline(os, line);
	while (line.find("server") == std::string::npos && !os.eof())
	{
		// line.erase(line.begin() + line.find("#"), line.end());
		// check comments
		if (line.find("#") != std::string::npos || has_only_spaces(line) || line.empty())
			continue;
		std::cout << line << std::endl;
		//erase all spaces
		line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
		// line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());
		if (line.empty())
			getline(os, line);
		else
			throw std::runtime_error("Error: ruller is not at the top of the file");
	}
	if (os.eof())
		throw std::runtime_error("Error: no server block");
	else if (line.find("server") != 0)
		throw std::runtime_error("Error: invalid server block");
	else
		server = true;
    while (std::getline(os, line))
    {
		// erase comments
		// line.erase(line.begin() + (line.find("#")), line.end());
		std::cout << line << std::endl;
		if (line.find("#") != std::string::npos || has_only_spaces(line) || line.empty())
			continue;
		// 	throw std::runtime_error("Error: empty line inside block");
		if (tab_count(line) != 1)
			throw std::runtime_error("Error: invalid indent server block");
		std::istringstream iss(line);
        std::string key, value;
        std::getline(iss, key, ' ');
        std::getline(iss, value);
		if (key != "location" && key != "server")
		{
			current_server[key] = value;
			herservers.push_back(current_server);
		}
		else if (key == "location")
		{
			location = true;
			current_server[key] = value;
			herservers.push_back(current_server);
			while (std::getline(os, line))
			{
				// line.erase(line.begin() + line.find("#"), line.end());
				if (line.find("location") != std::string::npos)
					throw std::runtime_error("Error: invalid location block");
				if ((line.find("server") != std::string::npos && track < 2) || (line.find("server") != std::string::npos && line.find("server") != 0))
					throw std::runtime_error("Error: invalid server block");
				else if (line.find("server") != std::string::npos)
				{
					server = true;
					location = false;
					track = 0;
					std::istringstream iss(line);
        			std::string key, value;
        			std::getline(iss, key, ' ');
        			std::getline(iss, value);
					current_server[key] = value;
					herservers.push_back(current_server);
					break;
				}
				else if (tab_count(line) != 2)
					break;
				else
				{
					std::istringstream iss(line);
        			std::string key, value;
        			std::getline(iss, key, ' ');
        			std::getline(iss, value);
					current_server[key] = value;
					herservers.push_back(current_server);
				}
				track++;
			}
		}
    }
	//  print servers
	for (std::vector<std::map<std::string, std::string> >::iterator it = herservers.begin(); it != herservers.end(); it++)
	{
		for (std::map<std::string, std::string>::iterator it2 = it->begin(); it2 != it->end(); it2++)
		{
			std::cout << it2->first << " " << it2->second << std::endl;
		}
	}
}