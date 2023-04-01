/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverconfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchliyah <mchliyah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 23:37:35 by mchliyah          #+#    #+#             */
/*   Updated: 2023/04/01 22:24:08 by mchliyah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

std::string serverconfig::getServerName() const{
	return (this->server_name);
}

std::string serverconfig::getListen() const {
	return (this->listen);
}

std::string serverconfig::getMaxClientBodySize() const {
	return (this->maxclientboddysize);
}

std::map<int, std::string> serverconfig::getErrorPages() const {
	return (this->errorpages);
}

std::map<std::string, locationconfig> serverconfig::getLocations() const {
	return (this->locations);
}

serverconfig::serverconfig(const serverconfig &src) {
	*this = src;
}

serverconfig &serverconfig::operator=(const serverconfig &src) {
	if (this == &src)
		return (*this);
	this->server_name = src.server_name;
	this->listen = src.listen;
	this->maxclientboddysize = src.maxclientboddysize;
	this->errorpages = src.errorpages;
	this->locations = src.locations;
	return (*this);
}

std::string serverconfig::readServer(std::ifstream& inputFile, std::string line) {
	std::string key, value;

	if (line.find("server") != std::string::npos)
	{
		if (line.length() != 6)
			throw std::runtime_error("Error: server block error");
		getline(inputFile, line);
		g_tab_count = tab_count(line);
	}
	while (g_tab_count == 1) 
	{
		if (has_only_spaces(line) || line.empty())
			throw std::runtime_error("Error: empty line inside server block\n \t or two or more empty lines");
		if (line.find("#") != std::string::npos)
			throw std::runtime_error("Error: comment is not allowed");
		if (line.find("location") != std::string::npos)
		{
			locationconfig location;
			value.clear();
			key.clear();
			std::istringstream iss(line);
			iss >> key >> value;
			if (value.empty())
				throw std::runtime_error("Error: location is empty");
			getline(inputFile, line);
			g_tab_count = tab_count(line);
			line = location.readlocation(inputFile, line);
			location.setName(value);
			locations.insert(std::make_pair(value, location));
			if (inputFile.eof() ||  (line.find("server") != std::string::npos && line.find("server") == 0))
				return (line);
			else
				continue;
		}
		else
		{
			value.clear();
			key.clear();
			std::istringstream iss(line);
			iss >> key >> value;
			if (value.empty())
				throw std::runtime_error("Error: server block has empty value");
			if (key != "server_name" && key != "listen" && key != "max_client_body_size" && key != "location" && key != "error_page")
					throw std::runtime_error("Error: server block has invalid key");
			if (key == "server_name") {
				server_name = value;
			}
			else if (key == "listen") {
				listen = value;
			}
			else if (key == "max_client_body_size") {
				maxclientboddysize = value;
			}
			else if (key == "error_page") {
				std::string errorpage;
				iss  >> errorpage;
				if (value.empty() || errorpage.empty())
					throw std::runtime_error("Error: error_page has less than 2 arguments");
				errorpages.insert(std::make_pair(atoi(value.c_str()), errorpage));
			}
			value.clear();
			iss >> value;
			if (!value.empty())
				throw std::runtime_error("Error: error_page has more than 2 arguments");
		}
		if (line.find("location") == std::string::npos)
			getline(inputFile, line);
		g_tab_count = tab_count(line);
		if (inputFile.eof() || (line.find("server") != std::string::npos && line.find("server") == 0))
			return (line);
		if (has_only_spaces(line) || line.empty())
			throw std::runtime_error("Error: empty line inside server block\n \t or two or more empty lines");
		value.clear();
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
	std::map<std::string, locationconfig>::iterator it2 = locations.begin();
	while (it2 != locations.end())
	{
		std::cout << "location: " << it2->first << std::endl;
		it2->second.printlocation();
		it2++;
	}
}

serverconfig::serverconfig() {
	server_name = "";
	listen = "";
	maxclientboddysize = "";
}

serverconfig::~serverconfig() {
	server_name.clear();
	listen.clear();
	maxclientboddysize.clear();
	errorpages.clear();
	locations.clear();

}
