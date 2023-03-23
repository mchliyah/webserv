/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 23:58:49 by slahrach          #+#    #+#             */
/*   Updated: 2023/03/22 23:59:27 by slahrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CONFIG_HPP
# define CONFIG_HPP
#include <iostream>
#include <stdlib.h>
#include <string>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdexcept>
#include <sys/select.h>
#include <vector>
#include <algorithm>
#include <utility>
#include <map>
#include <fstream>
#include <sstream>

extern int g_tab_count;

class locationconfig
{
	std::string root;
	std::string cgipass;
	std::string cgi_pass;
	std::string autoindex;
	std::vector<std::string> index;
	std::map<std::string, bool> allowsmethod;

	public:
		locationconfig();
		~locationconfig();
		std::string readlocation(std::ifstream& inputFile, std::string line);
		void printlocation();
		std::string getRoot() const;
		std::string getCgiPass() const;
		std::vector<std::string> getIndex() const;
		std::map<std::string, bool> getAllowsMethod() const;
		std::string getAutoIndex() const;
};

class serverconfig
{
		std::string server_name;
		std::map<std::string, locationconfig> locations;
		std::string listen;
		std::string maxclientboddysize;
		std::map<int, std::string> errorpages;
	public:
	serverconfig();
	~serverconfig();
	std::string readServer(std::ifstream& inputFile, std::string line);
	void printServer();
	std::string getServerName() const;
	std::string getListen() const;
	std::string getMaxClientBodySize() const;
	std::map<int, std::string> getErrorPages() const;
	std::map<std::string, locationconfig> getLocations() const;
	// serverconfig getServers() const;
};

std::vector<serverconfig> parse(std::string path);
bool has_only_spaces(std::string str);
int tab_count(std::string str);
#endif