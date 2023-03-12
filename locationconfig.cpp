/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   locationconfig.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchliyah <mchliyah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 23:37:32 by mchliyah          #+#    #+#             */
/*   Updated: 2023/03/12 19:27:30 by mchliyah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"



std::string locationconf::getRoot() const{
	return (this->root);
}

std::string locationconf::getCgiPass() const{
	return (this->cgipass);
}

std::map<std::string, bool> locationconf::getAllowsMethod() const{
	return (this->allowsmethod);
}

std::vector<std::string> locationconf::getIndex() const{
	return (this->index);
}


std::string locationconf::readlocation(std::ifstream& inputFile, std::string line) {
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
		if (key != "root" && key != "index" && key != "allow_method" && key != "cgi_pass")
			throw std::runtime_error("Error: location block key is not valid");
		if (key == "root") {
			root = value;
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
		else if (key == "cgi_pass") {
			std::cout << "cgi value: " << value << std::endl;
			cgipass = value;
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


void locationconf::printlocation() {
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
