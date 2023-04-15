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

std::string& locationconfig::readlocation(std::ifstream& inputFile, std::string& line) {
	std::string key, value;
	while (g_tab_count == 2 && !inputFile.eof())
	{
		if (line.find("#") != std::string::npos || line.find(";") != std::string::npos)
			throw std::runtime_error("Error: invalid caracter in the file");
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
		if (key != "root" && key != "index" && key != "allow_method" && key != "cgi_pass" && key != "autoindex" && key != "upload_store" && key != "return")
			throw std::runtime_error("Error: location block key is not valid");
		if (key == "root")
			root = value;
		else if (key == "return")
			redirect = value;
		else if (key == "cgi_pass")
			cgi_pass = value;
		else if (key == "upload_store")
			upload_store = value;
		else if (key == "autoindex")
			autoindex = value;
		else if (key == "index")
			while (!value.empty()) {
				index.push_back(value);
				value.clear();
				iss >> value;
			}
		else if (key == "allow_method")
			while (!value.empty()) {
				if (value != "GET" && value != "POST"  && value != "DELETE")
					throw std::runtime_error("Error: allow_method value is not valid");
				allowsmethod[value] = true;
				value.clear();
				iss >> value;
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
	std::cout << "location name: " << loc_name << std::endl;
	std::cout << "	root: " << root << std::endl;
	std::cout << "	cgi_pass: " << cgi_pass << std::endl;
	std::cout << "	autoindex: " << autoindex << std::endl;
	std::cout << "	upload_store: " << upload_store << std::endl;
	std::cout << "	return : " << redirect << std::endl;
	for (std::vector<std::string>::iterator it = index.begin(); it != index.end() ; it++)
		std::cout << "	index: " << *it << std::endl;
	for (std::map<std::string, bool>::iterator it = allowsmethod.begin(); it != allowsmethod.end(); it++)
		std::cout << "	allow_method: " << it->first << " " << it->second << std::endl;
	std::cout << "========= " << std::endl;
}

locationconfig::locationconfig(const locationconfig &other) {
	*this = other;
}

locationconfig &locationconfig::operator=(const locationconfig &other) {
	if (this != &other)
	{
		this->loc_name = other.loc_name;
		this->root = other.root;
		this->cgi_pass = other.cgi_pass;
		this->autoindex = other.autoindex;
		this->index = other.index;
		this->allowsmethod = other.allowsmethod;
		this->upload_store = other.upload_store;
		this->redirect = other.redirect;
	}
	return (*this);
}

locationconfig::locationconfig() {
	this->loc_name = "";
	this->root = "";
	this->cgi_pass = "";
	this->autoindex = "";
	this->upload_store = "";
	this->redirect = "";
	allowsmethod["GET"] = false;
	allowsmethod["POST"] = false;
	allowsmethod["DELETE"] = false;
	index.clear();
}

std::string& locationconfig::getRoot(){
	return (this->root);
}

std::string& locationconfig::getCgiPass(){
	return (this->cgi_pass);
}

std::map<std::string, bool>& locationconfig::getAllowsMethod(){
	return (this->allowsmethod);
}

std::vector<std::string>& locationconfig::getIndex(){
	return (this->index);
}

std::string& locationconfig::getAutoIndex(){
	return (this->autoindex);
}
std::string& locationconfig::getName(){
	return (this->loc_name);
}
void locationconfig::setName(std::string name){
	this->loc_name = name;
}

std::string& locationconfig::getUploadStore(){
	return (this->upload_store);
}

std::string& locationconfig::getRedirect(){
	return (this->redirect);
}

void locationconfig::setRoot(std::string root){
	this->root = root;
}

void locationconfig::setCgiPass(std::string cgi_pass){
	this->cgi_pass = cgi_pass;
}

void locationconfig::setAllowsMethod(std::map<std::string, bool>& allowsmethod){
	this->allowsmethod = allowsmethod;
}

void locationconfig::setIndex(std::vector<std::string>& index){
	this->index = index;
}

void locationconfig::setAutoIndex(std::string autoindex){
	this->autoindex = autoindex;
}

locationconfig::~locationconfig() {
	loc_name.clear();
	root.clear();
	index.clear();
	allowsmethod.clear();
	cgi_pass.clear();
	autoindex.clear();
	upload_store.clear();
	redirect.clear();
}