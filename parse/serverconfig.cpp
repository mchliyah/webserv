/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   serverconfig.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 23:37:35 by mchliyah          #+#    #+#             */
/*   Updated: 2023/04/14 00:41:59 by slahrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

std::string& serverconfig::getServerName(){
	return (this->server_name);
}

std::string& serverconfig::getListen() {
	return (this->listen);
}

std::string& serverconfig::getMaxClientBodySize() {
	return (this->maxclientboddysize);
}

std::string& serverconfig::getHostName() {
	return (this->host_name);
}

std::map<std::string, std::string>& serverconfig::getErrorPages() {
	return (this->errorpages);
}

std::map<std::string, locationconfig>& serverconfig::getLocations(){
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
	this->default_page = src.default_page;
	this->host_name = src.host_name;
	return (*this);
}

std::string& serverconfig::readServer(std::ifstream& inputFile, std::string& line) {
	std::string key, value;

	if (line.find("server") != std::string::npos)
	{
		if (line.length() != 6)
			throw std::runtime_error("Error: server block error");
		getline(inputFile, line);
		g_tab_count = tab_count(line);
	}
	while (g_tab_count == 1 && !inputFile.eof()) 
	{
		if (has_only_spaces(line) || line.empty())
			throw std::runtime_error("Error: empty line inside server block\n \t or line with only spaces");
		if (line.find("#") != std::string::npos || line.find(";") != std::string::npos)
			throw std::runtime_error("Error: invalid caracter in the file");
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
			if (key != "server_name" && key != "listen" && key != "max_client_body_size" && key != "location" && key != "error_page" && key != "host")
					throw std::runtime_error("Error: server block has invalid key");
			if (key == "server_name") {
				server_name = value;
			}
			else if (key == "host") {
				host_name = value;
			}
			else if (key == "listen") {
				if (value.find_first_not_of("0123456789") != std::string::npos)
					throw std::runtime_error("Error: put a valid port number");
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
				errorpages.insert(std::make_pair(value, errorpage));
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
	std::cout << "host: " << host_name << std::endl;
	std::cout << "max_client_body_size: " << maxclientboddysize << std::endl;
	for (std::map<std::string, std::string>::iterator it = errorpages.begin(); it != errorpages.end(); it++)
		std::cout << "error_page: " << it->first << " " << it->second << std::endl;
	for (std::map<std::string, locationconfig>::iterator it = locations.begin(); it != locations.end(); it++)
	{
		std::cout << "location: " << it->first << std::endl;
		it->second.printlocation();
	}
}

serverconfig::serverconfig() {
	server_name = "";
	listen = "";
	host_name = "localhost";
	maxclientboddysize = "";
	default_page.clear();
	errorpages.clear();
	locations.clear();
}

std::map<std::string, std::string>& serverconfig::getDefaultPage() {
	return default_page;
}

void serverconfig::setServerName(std::string name) {
	server_name = name;
}

void serverconfig::setListen(std::string listen) {
	this->listen = listen;
}

void serverconfig::setLocations(std::map<std::string, locationconfig>& loc) {
	locations = loc;
}

void serverconfig::setHostName(std::string host) {
	host_name = host;
}

serverconfig::~serverconfig() {
	server_name.clear();
	listen.clear();
	maxclientboddysize.clear();
	errorpages.clear();
	locations.clear();
	default_page.clear();
}
