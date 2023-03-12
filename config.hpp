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

class locationconf
{
	std::string root;
	std::string cgipass;
	std::vector<std::string> index;
	std::map<std::string, bool> allowsmethod;
	std::string cgi_pass;

	public:
		std::string readlocation(std::ifstream& inputFile, std::string line);
		void printlocation();
		std::string getRoot() const;
		std::string getCgiPass() const;
		std::vector<std::string> getIndex() const;
		std::map<std::string, bool> getAllowsMethod() const;
};

class serverconfig
{
		std::string server_name;
		std::map<std::string, locationconf> locations;
		int listen;
		int maxclientboddysize;
		std::map<int, std::string> errorpages;
	public:
	std::string readServer(std::ifstream& inputFile, std::string line);
	void printServer();
	std::string getServerName() const;
	int getListen() const;
	int getMaxClientBodySize() const;
	std::map<int, std::string> getErrorPages() const;
	std::map<std::string, locationconf> getLocations() const;
};

void parse(std::string path);
bool has_only_spaces(std::string str);
int tab_count(std::string str);
