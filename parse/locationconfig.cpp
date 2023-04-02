/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locationconfigig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchliyah <mchliyah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 23:37:32 by mchliyah          #+#    #+#             */
/*   Updated: 2023/03/15 17:08:41 by mchliyah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

std::string locationconfig::readlocation(std::ifstream& inputFile, std::string line) {
	std::string key, value;
	std::pair<std::string, std::string> pair;

	while (g_tab_count == 2)
	{
		if (line.find("#") != std::string::npos)
			throw std::runtime_error("Error: comment is not allowed");
		if (has_only_spaces(line) || line.empty())
			continue;
		if (line.find("location") != std::string::npos || (line.find("server") != std::string::npos && line.length() == 6))
			throw std::runtime_error("error: nested location");
		key.clear();
		value.clear();
		std::istringstream iss(line);
		iss >> key >> value;
		if (value.empty() || value == "")
			throw std::runtime_error("Error: location block value is empty");
		if (key != "root" && key != "index" && key != "allow_method" && key != "cgi_pass" && key != "autoindex")
			throw std::runtime_error("Error: location block key is not valid");
		if (key == "root") {
			root = value;
		}
		else if (key == "cgi_pass") {
			cgipass = value;
		}
		else if (key == "autoindex"){
			autoindex = value;
		}
		else if (key == "index") {
			while (!value.empty()) {
				index.push_back(value);
				value.clear();
				iss >> value;
			}
		}
		else if (key == "allow_method") {
			while (!value.empty()) {
				allowsmethod.insert(std::make_pair(value, true));
				value.clear();
				iss >> value;
			}
		}
		value.clear();
		iss >> value;
		if (!value.empty())
			throw std::runtime_error("Error: value argument is more than 2");
		std::getline(inputFile, line);
		g_tab_count = tab_count(line);
		value.clear();
	}
	return (line);
}


void locationconfig::printlocation() {
	std::cout << "	root: " << root << std::endl;
	std::cout << "	cgi_pass: " << cgipass << std::endl;
	std::cout << "	autoindex: " << autoindex << std::endl;
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
}

locationconfig::locationconfig(const locationconfig &other) {
	*this = other;
}

locationconfig &locationconfig::operator=(const locationconfig &other) {
	if (this != &other)
	{
		this->loc_name = other.loc_name;
		this->root = other.root;
		this->cgipass = other.cgipass;
		this->cgi_pass = other.cgi_pass;
		this->autoindex = other.autoindex;
		this->index = other.index;
		this->allowsmethod = other.allowsmethod;
	}
	return (*this);
}

locationconfig::locationconfig() {
	this->loc_name = "";
	this->root = "";
	this->cgipass = "";
	this->cgi_pass = "";
	this->autoindex = "";
}

std::string locationconfig::getRoot() const{
	return (this->root);
}

std::string locationconfig::getCgiPass() const{
	return (this->cgipass);
}

std::map<std::string, bool> locationconfig::getAllowsMethod() const{
	return (this->allowsmethod);
}

std::vector<std::string> locationconfig::getIndex() const{
	return (this->index);
}

std::string locationconfig::getAutoIndex() const{
	return (this->autoindex);
}
std::string locationconfig::getName() const{
	return (this->loc_name);
}
void locationconfig::setName(std::string name){
	this->loc_name = name;
}

locationconfig::~locationconfig() {
	loc_name.clear();
	root.clear();
	index.clear();
	allowsmethod.clear();
	cgipass.clear();
	autoindex.clear();
}