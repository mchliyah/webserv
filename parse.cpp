#include "server.hpp"

class locationconf
{
	std::string root;
	std::string cgipass;
	std::vector<std::string> index;
	std::map<std::string, bool> allowsmethod;
	//auto index 
	//apllod pass


	public:

	void setRoot(const std::string& str){

	}

	void setIndexs(const std::string& str){

	}

	

}

class sercverconfig
{
	private
		std::string server_name
		std::map<std::string locationconf> locations;
		int listen;
		int maxclientboddysize;
		std::vactor<int, std::string> errorpages;

	void readServer(std::ifstream& inputFile){

	}
	
}

bool has_only_spaces(std::string str)
{
	std::string::iterator it = str.begin();
	while (it != str.end())
	{
		if (*it != ' ' && *it != '\t')
			return false;
		it++;
	}
	return true;
}

int tab_count(std::string str) {
	return std::count(str.begin(), str.end(), '\t');
}

void parse(std::string path){
	std::string line;
    std::ifstream os(path);
	sercverconfig server;
	std::vector<serverconfig> servers;
	// std::map<std::string, std::vector> m;

	// m["dsfsd"] = std::vector<std::string>();
	// m["dsfsd"].insert() // insert to vector refernced tp dsfsd
	// m.insert() //insert new pair string && vector
	// m.earse()
	// m.find() // return 

	if (!os.is_open())
		throw std::runtime_error("Error: can't open config file");
	while (std::getline(os, line))
	{
		// erase comments
		line.erase(line.begin() + (line.find("#")), line.end());
		std<<cout<<line<<std::endl;
		// if (line.find("#") != std::string::npos || has_only_spaces(line) || line.empty())
		// 	continue;
		// // erase all spaces
		// line.erase(std::remove(line.begin(), line.end(), ' '), line.end());
		// if (line.empty())
		// 	getline(os, line);
		// else
		// 	throw std::runtime_error("Error: ruller is not at the top of the file");
	}
}