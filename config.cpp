#include "server.hpp"

config::config() {
	server_name = "";
	listen = "";
	root = "";
	index = "";
	error_page = "";
	location = "";
	autoindex = "";
	client_max_body_size = "";
	return_ = "";
	cgi_pass = "";
}

void config::setServerName(std::string server_name) {
	if (server_name != "")
		this->server_name = server_name;
}

void config::setListen(std::string listen) {
	if (listen != "")
		this->listen = listen;
}

void config::setRoot(std::string root) {
	if (root != "")	
		this->root = root;
}

void config::setIndex(std::string index) {
	if (index != "")
		this->index = index;
}

void config::setErrorPage(std::string error_page) {
	if (error_page != "")
		this->error_page = error_page;
}

void config::setLocation(std::string location) {
	if (location != "")
		this->location = location;
}

void config::setAutoindex(std::string autoindex) {
	if (autoindex != "")
		this->autoindex = autoindex;
}

void config::setClientMaxBodySize(std::string client_max_body_size) {
	if (client_max_body_size != "")
		this->client_max_body_size = client_max_body_size;
}

void config::setReturn(std::string return_) {
	if (return_ != "")
		this->return_ = return_;
}

void config::setCgiPass(std::string cgi_pass) {
	if (cgi_pass != "")
		this->cgi_pass = cgi_pass;
}

void config::setBuffer(std::vector<std::string> buffer) {
	this->buffer = buffer;
}

std::string config::getServerName() {
	return server_name;
}

std::string config::getListen() {
	return listen;
}

std::string config::getRoot() {
	return root;
}

std::string config::getIndex() {
	return index;
}

std::string config::getErrorPage() {
	return error_page;
}

std::string config::getLocation() {
	return location;
}

std::string config::getAutoindex() {
	return autoindex;
}

std::string config::getClientMaxBodySize() {
	return client_max_body_size;
}

std::string config::getReturn() {
	return return_;
}

std::vector<std::string> config::getBuffer() {
	return buffer;
}

void	config::printConfig(){
	std::cout << "server_name: " << server_name << std::endl;
	std::cout << "listen: " << listen << std::endl;
	std::cout << "root: " << root << std::endl;
	std::cout << "index: " << index << std::endl;
	std::cout << "error_page: " << error_page << std::endl;
	std::cout << "location: " << location << std::endl;
	std::cout << "autoindex: " << autoindex << std::endl;
	std::cout << "client_max_body_size: " << client_max_body_size << std::endl;
	std::cout << "return: " << return_ << std::endl;
	std::cout << "cgi_pass: " << cgi_pass << std::endl;

}

config::~config() {
}