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
	int count = 0;
	while (str[count] == '\t')
		count++;
	return (count);
}

std::string serverconfig::readServer(std::ifstream& inputFile, std::string line) {
	std::string key, value;
	std::pair<std::string, std::string> pair;
	int i = 0;

	if (line.find("server") != std::string::npos)
	{
		getline(inputFile, line);
		g_tab_count = tab_count(line);
	}
	while (g_tab_count == 1) 
	{
		// std::cout << "line: " << line << std::endl;
		if (line.find("#") != std::string::npos)
			throw std::runtime_error("Error: comment is not allowed");
		if (has_only_spaces(line) || line.empty())
			continue;
		if (line.find("location") != std::string::npos)
		{
			locationconf location;
			std::istringstream iss(line);
			iss >> key >> value;
			pair = std::make_pair(key, value);
			getline(inputFile, line);
			g_tab_count = tab_count(line);
			line = location.readlocation(inputFile, line);
			locations.insert(std::make_pair(pair.second, location));
			if (inputFile.eof() ||  (line.find("server") != std::string::npos && line.find("server") == 0))
				return (line);
		}
		else
		{
			std::istringstream iss(line);
			iss >> key >> value;
			pair = std::make_pair(key, value);
			if (pair.first == "server_name")
				server_name = pair.second;
			else if (pair.first == "listen")
				listen = atoi(pair.second.c_str());
			else if (pair.first == "max_client_body_size")
				maxclientboddysize = atoi(pair.second.c_str());
			else if (pair.first == "error_page")
			{
				std::string errorcode;
				std::string errorpage;
				std::istringstream iss(pair.second);
				iss >> errorcode >> errorpage;
				errorpages.insert(std::make_pair(atoi(errorcode.c_str()), errorpage));
			}
		}
		std::getline(inputFile, line);
		g_tab_count = tab_count(line);
		if (inputFile.eof() || (line.find("server") != std::string::npos && line.find("server") == 0))
			return (line);
	}
	return (line);
	}

std::string locationconf::readlocation(std::ifstream& inputFile, std::string line) {
	std::string key, value;
	std::pair<std::string, std::string> pair;

	while (g_tab_count == 2)
	{
		// std::cout << line << std::endl;
		if (line.find("#") != std::string::npos)
			throw std::runtime_error("Error: comment is not allowed");
		if (has_only_spaces(line) || line.empty())
			continue;
		if (line.find("location") != std::string::npos || (line.find("server") != std::string::npos && line.length() == 6))
			throw std::runtime_error("error: nested location");
		std::istringstream iss(line);
		iss >> key >> value;
		pair = std::make_pair(key, value);
		if (pair.first == "root")
			root = pair.second;
		else if (pair.first == "index")
			index.push_back(pair.second);
		else if (pair.first == "allow_method")
			allowsmethod.insert(std::make_pair(pair.second, true));
		else if (pair.first == "cgi_pass")
			cgipass = pair.second;
		std::getline(inputFile, line);
		g_tab_count = tab_count(line);
	}
	return (line);
}

void serverconfig::printServer() {
	std::cout << "server_name: " << server_name << std::endl;
	std::cout << "listen: " << listen << std::endl;
	std::cout << "max_client_body_size: " << maxclientboddysize << std::endl;
	std::map<int, std::string>::iterator it = errorpages.begin();
	while (it != errorpages.end())
	{
		std::cout << "error_page: " << it->first << " " << it->second << std::endl;
		it++;
	}
	std::map<std::string, locationconf>::iterator it2 = locations.begin();
	while (it2 != locations.end())
	{
		std::cout << "location: " << it2->first << std::endl;
		it2->second.printlocation();
		it2++;
	}
}

void locationconf::printlocation()
{
	std::cout << "	root: " << root << std::endl;
	std::vector<std::string>::iterator it = index.begin();
	while (it != index.end())
	{
		std::cout << "	index: " << *it << std::endl;
		it++;
	}
	std::map<std::string, bool>::iterator it2 = allowsmethod.begin();
	while (it2 != allowsmethod.end())
	{
		std::cout << "	allow_method: " << it2->first << std::endl;
		it2++;
	}
	std::cout << "	cgi_pass: " << cgipass << std::endl;
}
