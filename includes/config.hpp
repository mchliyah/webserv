/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   config.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchliyah <mchliyah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/22 23:58:49 by slahrach          #+#    #+#             */
/*   Updated: 2023/04/04 04:47:57 by mchliyah         ###   ########.fr       */
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
#include <sys/stat.h>
#include <dirent.h>

extern int g_tab_count;

class locationconfig
{
	std::string loc_name;
	std::string root;
	std::string cgi_pass;
	std::string autoindex;
	std::string upload_store;
	std::vector<std::string> index;
	std::map<std::string, bool> allowsmethod;

	public:
		locationconfig();
		locationconfig(const locationconfig& other);
		locationconfig& operator=(const locationconfig& other);
		~locationconfig();
		std::string readlocation(std::ifstream& inputFile, std::string line);
		void printlocation();
		void setName(std::string name);
		std::string& getRoot();
		std::string& getCgiPass();
		std::vector<std::string> getIndex() const;
		std::map<std::string, bool> getAllowsMethod() const;
		std::string getAutoIndex() const;
		std::string getName() const;
		std::string getUploadStore() const;
};

class serverconfig
{
	private:
		std::string server_name;
		std::map<std::string, locationconfig> locations;
		std::string listen;
		std::string maxclientboddysize;
		std::map<int, std::string> errorpages;
	public:
	serverconfig();
	serverconfig(const serverconfig& other);
	serverconfig& operator=(const serverconfig& other);
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