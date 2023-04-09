/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchliyah <mchliyah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 16:51:42 by mchliyah          #+#    #+#             */
/*   Updated: 2023/04/09 09:16:10 by mchliyah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

void response::clearall() {
	status_code = "";
	status_message = "";
	content_type = "";
	header = "";
	body = "";
	content_length = "";
	date = "";
}

void response::clear() {
	header = "";
	body = "";
}

response::response() {
	std::time_t t = std::time(nullptr);
	char buffer[128];
	std::strftime(buffer, sizeof(buffer), "%a, %d %b %Y %H:%M:%S %Z", std::localtime(&t));
	this->date = "Date: " + std::string(buffer) + "\r\n";
	status_code_map["200"] = "OK";
	status_code_map["201"] = "Created";
	status_code_map["204"] = "No Content";
	status_code_map["400"] = "Bad Request";
	status_code_map["401"] = "Unauthorized";
	status_code_map["403"] = "Forbidden";
	status_code_map["404"] = "Not Found";
	status_code_map["405"] = "Method Not Allowed";
	status_code_map["413"] = "Payload Too Large";
	status_code_map["412"] = "Precondition Failed";
	status_code_map["414"] = "URI Too Long";
	status_code_map["415"] = "Unsupported Media Type";
	status_code_map["500"] = "Internal Server Error";
	status_code_map["501"] = "Not Implemented";
	status_code_map["505"] = "HTTP Version Not Supported";
}

response::response(const response &src) {
	*this = src;
}

response &response::operator=(const response &src) {
	if (this != &src) {
		this->status_code = src.status_code;
		this->status_message = src.status_message;
		this->content_type = src.content_type;
		this->header = src.header;
		this->body = src.body;
		this->content_length = src.content_length;
		this->date = src.date;
		this->status_code_map = src.status_code_map;
	}
	return *this;
}


std::string response::get_status_code() { return status_code; }

std::string response::get_status_message() { return status_message; }

std::string response::get_content_type() { return content_type; }

std::string response::get_content() { return header; }

std::string response::get_content_length() { return content_length; }

std::string response::get_date() { return date; }

std::string response::get_body(void ) { return body; }

void response::set_body(std::string body) { this->body = body; }

void response::set_header(std::string content) { this->header = content; }

void response::set_content_type(std::string content_type) { this->content_type = content_type; }

void response::set_content_length(std::string content_length) { this->content_length = content_length; }

void response::set_status_code(std::string status_code) { this->status_code = status_code; }

void response::set_status_message(std::string status_message) { this->status_message = status_message; }

void response::add_to_header(std::string header) { this->header += header ; }

void response::clear_header(void) { this->header = ""; }

std::string response::get_header(void) const { return (this->header); }

std::map<std::string, std::string> response::get_status_code_map() { return status_code_map; }


response::~response() { 
	
}
