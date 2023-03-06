#include "server.hpp"

void parse(std::string path)
{
	std::string line;
	std::vector<std::string> buffer;
	std::ifstream os(path);
	int count;
	config conf;

	if (!os.is_open())
		throw std::runtime_error("Error: can't open config file");
	while (std::getline(os, line))
	{
		//check for comments
		if (line.find("#") != std::string::npos)
			continue;
		//skip spaces and tabs
		line.erase(std::remove(line.begin(), line.end(), '\t'), line.end());
		// line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
		if (line.find("\t") != std::string::npos)
			std::cout << "tabe" << std::endl;
		if (!line.empty())
			buffer.push_back(line);
	}
	conf.setBuffer(buffer);
	// print buffer
	// for (std::vector<std::string>::iterator it = buffer.begin(); it != buffer.end(); it++)
	// 	std::cout << *it << std::endl;

	// set server name
	for (std::vector<std::string>::iterator it = buffer.begin(); it != buffer.end(); it++)
	{
		conf.setServerName(it->find("server_name") != std::string::npos ? it->substr(it->find("server_name") + 11) : "");
		conf.setListen(it->find("listen") != std::string::npos ? it->substr(it->find("listen") + 6) : "");
		conf.setRoot(it->find("root") != std::string::npos ? it->substr(it->find("root") + 4) : "");
		conf.setIndex(it->find("index") != std::string::npos ? it->substr(it->find("index") + 5) : "");
		conf.setErrorPage(it->find("error_page") != std::string::npos ? it->substr(it->find("error_page") + 10) : "");
		conf.setLocation(it->find("location") != std::string::npos ? it->substr(it->find("location") + 8) : "");
		conf.setAutoindex(it->find("autoindex") != std::string::npos ? it->substr(it->find("autoindex") + 9) : "");
		conf.setClientMaxBodySize(it->find("client_max_body_size") != std::string::npos ? it->substr(it->find("client_max_body_size") + 20) : "");
		conf.setReturn(it->find("return") != std::string::npos ? it->substr(it->find("return") + 6) : "");
		conf.setCgiPass(it->find("cgi_pass") != std::string::npos ? it->substr(it->find("cgi_pass") + 8) : "");


	}

	// printconfig
	conf.printConfig();

	// std::cout << buffer << std::endl;

}