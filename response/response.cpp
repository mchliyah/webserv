/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchliyah <mchliyah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 16:51:42 by mchliyah          #+#    #+#             */
/*   Updated: 2023/03/23 00:30:59 by mchliyah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"


std::string response::get_status_code() { return status_code; }

std::string response::get_status_message() { return status_message; }

std::string response::get_content_type() { return content_type; }

std::string response::get_content() { return content; }

std::string response::get_content_length() { return content_length; }

std::string response::get_date() { return date; }

std::vector<std::string> response::get_headers() { return headers; }

void response::add_header(std::string header) { headers.push_back(header); }

void response::set_content(std::string content) { this->content = content; }

void response::set_content_type(std::string content_type) { this->content_type = content_type; }

void response::set_content_length(std::string content_length) { this->content_length = content_length; }

void response::set_status_code(std::string status_code) { this->status_code = status_code; }

void response::set_status_message(std::string status_message) { this->status_message = status_message; }


// response::response(void) {
// 	this->status = "HTTP/1.1 200 OK\r\n";
// 	this->content_type = "Content-Type: text/plain\r\n";
// 	this->content_length = "Content-Length: 41\r\n";
// 	this->content = "\r\nHello, world! will i need to change this?";
// 	this->resp = this->status + this->content_type + this->content_length + this->content;
// }


response::response(const std::string& request_type) {
	std::time_t t = std::time(nullptr);
	char buffer[128];
	std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S %Z", std::localtime(&t));
	this->date = "Date: " + std::string(buffer) + "\r\n";
	if (request_type == "GET") {
	    status_code = "200";
	    status_message = "OK";
	} else if (request_type == "POST") {
	    status_code = "201";
	    status_message = "Created";
	} else if (request_type == "DELETE") {
	    status_code = "204";
	    status_message = "No Content";
	}
	content_type = "Content-Type: text/plain\r\n";
	content_length = "Content-Length: ";
}
std::string response::get_response(serverconfig &server, std::string &path) {
	std::cout << "path: " << path << std::endl;
	std::string response = "HTTP/1.1 " + status_code + " " + status_message + "\r\n";
	std::map<std::string, locationconfig> locations = server.getLocations();
	std::map<std::string, locationconfig>::iterator loc;
	std::string frst;
	std::string last;
	std::string::size_type pos = path.find_last_of('/');
	if (pos != std::string::npos) {
		frst = path.substr(0, pos);
		last = path.substr(pos + 1);
	}
	std::cout << "frst: " << frst << std::endl;
	std::cout << "last: " << last << std::endl;

	for (loc = locations.begin() ; loc != locations.end() ; loc++) {
		std::cout << "loc->first: " << loc->first << std::endl;
		// if (loc->first == path) {
		// 	if (loc->second.getAutoIndex() == "on") {
		// 		content += "autoindex";
		// 		content_length += std::to_string(content.length()) + "\r\n";
		// 		response += date;
		// 		response += content_type;
		// 		response += content_length;
		// 		std::vector<std::string>::iterator it;
		// 		for (it = headers.begin(); it != headers.end(); it++)
		// 			response += *it;
		// 		response += "\r\n";
		// 		response += content;
		// 		return response;
		// 	}
		// }
	}
	content += "content_str";
	content_length += std::to_string(content.length()) + "\r\n";
	response += date;
	response += content_type;
	response += content_length;
	std::vector<std::string>::iterator it;
	for (it = headers.begin(); it != headers.end(); it++)
		response += *it;
	response += "\r\n";
	response += content;
	return response;
}

std::string response::post_response(serverconfig &server, std::string &path, std::string &body) {
	(void)server;
	(void)path;
	(void)body;
	std::string response = "HTTP/1.1 " + status_code + " " + status_message + "\r\n";
	content += "success";
	content_length += std::to_string(content.length()) + "\r\n";
	response += date;
	response += content_type;
	response += content_length;
	std::vector<std::string>::iterator it;
	for (it = headers.begin(); it != headers.end(); it++)
		response += *it;
	response += "\r\n";
	response += content;
	return response;
}

std::string response::delete_response(serverconfig &server, std::string &path) {
	(void)server;
	(void)path;
	std::string response = "HTTP/1.1 " + status_code + " " + status_message + "\r\n";
	content += "success";
	content_length += std::to_string(content.length()) + "\r\n";
	response += date;
	response += content_type;
	response += content_length;
	std::vector<std::string>::iterator it;
	for (it = headers.begin(); it != headers.end(); it++)
		response += *it;
	response += "\r\n";
	response += content;
	return response;
}

// std::ifstream file("test.txt");
// std::string responce;
// if (!file.is_open())
// 	throw std::runtime_error("cant open file 1");
// std::string str((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
std::string process_request(void) {
	// response resp("GET");
	// return response("POST");
    std::string response = "HTTP/1.1 200 OK\r\n";
    response += "Content-Type: text/plain\r\n";
    response += "Content-Length: 41\r\n";
    response += "\r\n";
    response += "Hello, world! will i need to change this?";
    return response;
}

response::~response() { }
