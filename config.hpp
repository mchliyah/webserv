
class server
{
	private:
		std::string server_name;
		std::vector<std::string> ports;
		std::map<std::string, std::string> localtion;
		std::vector<std::string> methods;
		std::vector<std::pair<int, std::string> > listners;
		std::vector<client> clients;
		std::vector<std::string> config_file;
		std::ifstream config;
	public:
		server(std::string path);
		server(std::vector<std::string> ports_);
		void start();
		std::pair<int,std::string> createBindListen(std::string port);
		std::ifstream GetConfige() const;
		~server();
};

class config {
	private:
		std::map<std::string, server> server; // server_name, listen
	public:
		config();
		~config();

		void printconfig();
};

void parse(std::string path);