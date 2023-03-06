
class config
{
	private:
		std::vector<std::string> buffer;
		std::string server_name;
		std::string listen;
		std::string root;
		std::string index;
		std::string error_page;
		std::string location;
		std::string autoindex;
		std::string client_max_body_size;
		std::string return_;
		std::string cgi_pass;
	public:
		config();
		~config();

		void setServerName(std::string server_name);
		void setListen(std::string listen);
		void setRoot(std::string root);
		void setIndex(std::string index);
		void setErrorPage(std::string error_page);
		void setLocation(std::string location);
		void setAutoindex(std::string autoindex);
		void setClientMaxBodySize(std::string client_max_body_size);
		void setReturn(std::string return_);
		void setCgiPass(std::string cgi_pass);
		void setBuffer(std::vector<std::string> buffer);
		std::string getServerName();
		std::string getListen();
		std::string getRoot();
		std::string getIndex();
		std::string getErrorPage();
		std::string getLocation();
		std::string getAutoindex();
		std::string getClientMaxBodySize();
		std::string getReturn();
		std::string getCgiPass();
		std::vector<std::string> getBuffer();

		void printConfig();
};

void parse(std::string path);