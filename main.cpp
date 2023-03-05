/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/25 05:09:29 by slahrach          #+#    #+#             */
/*   Updated: 2023/03/03 21:11:04 by slahrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "config.hpp"

void parce(std::string path)
{
	std::string line;
	std::ifstream os(path);
	config conf;

	if (!os.is_open())
		throw std::runtime_error("Error: can't open config file");
	while (std::getline(os, line))
	{

		//check for comments
		if (line.find("#") != std::string::npos || line.empty())
			continue;
		//skip spaces
		line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
		//check error
		if (line.find("error_page") != std::string::npos && line.find("error_page") != 0)
			throw std::runtime_error("Error: error_page must be at the beginning of the line");
		else if (line.find("location") != std::string::npos && line.find("location") != 0)
			throw std::runtime_error("Error: location must be at the beginning of the line");
		else if (line.find("return") != std::string::npos && line.find("return") != 0)
			throw std::runtime_error("Error: return must be at the beginning of the line");
		else if (line.find("cgi_pass") != std::string::npos && line.find("cgi_pass") != 0)
			throw std::runtime_error("Error: cgi_pass must be at the beginning of the line");
		// check for syntax error
		if (line.find("server_name") != std::string::npos && line.find("server_name") != 0)
			throw std::runtime_error("Error: server_name must be at the beginning of the line");
		else if (line.find("listen") != std::string::npos && line.find("listen") != 0)
			throw std::runtime_error("Error: listen must be at the beginning of the line");
		else if (line.find("root") != std::string::npos && line.find("root") != 0)
			throw std::runtime_error("Error: root must be at the beginning of the line");
		else if (line.find("index") != std::string::npos && line.find("index") != 0)
			throw std::runtime_error("Error: index must be at the beginning of the line");
		else if (line.find("autoindex") != std::string::npos && line.find("autoindex") != 0)
			throw std::runtime_error("Error: autoindex must be at the beginning of the line");
		else if (line.find("client_max_body_size") != std::string::npos && line.find("client_max_body_size") != 0)
			throw std::runtime_error("Error: client_max_body_size must be at the beginning of the line");

		//checke for server name and port
		conf.setServerName(line.find("server_name") != std::string::npos ? line.substr(line.find("server_name") + 11) : "");
		conf.setListen(line.find("listen") != std::string::npos ? line.substr(line.find("listen") + 6) : "");
		conf.setRoot(line.find("root") != std::string::npos ? line.substr(line.find("root") + 4) : "");
		conf.setIndex(line.find("index") != std::string::npos ? line.substr(line.find("index") + 5) : "");
		conf.setErrorPage(line.find("error_page") != std::string::npos ? line.substr(line.find("error_page") + 10) : "");
		conf.setLocation(line.find("location") != std::string::npos ? line.substr(line.find("location") + 8) : "");
		conf.setAutoindex(line.find("autoindex") != std::string::npos ? line.substr(line.find("autoindex") + 9) : "");
		conf.setClientMaxBodySize(line.find("client_max_body_size") != std::string::npos ? line.substr(line.find("client_max_body_size") + 20) : "");
		conf.setCgiPass(line.find("cgi_pass") != std::string::npos ? line.substr(line.find("cgi_pass") + 8) : "");

	}
	conf.printConfig();
}

int main(int argc, char **argv)
{
	try
	{
		// std::string s = "    GET     kkk      a     \r\nHost:n\r\nContent-Type:mm\r\n\r\nbodyyyy";
		// request req(s);
		// std::cout << req.getError() << "   " << req.getErrorMessage() << std::endl;
		// req.printAttr();
		parce(argv[1]);
		// std::vector<std::string> v;
		// v.push_back("80");
		// v.push_back("3000");
		// v.push_back("9000");
		// v.push_back("5000");
		// server my(v);
		// my.start();
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
}