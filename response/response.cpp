/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchliyah <mchliyah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 16:51:42 by mchliyah          #+#    #+#             */
/*   Updated: 2023/03/21 17:14:04 by mchliyah         ###   ########.fr       */
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
	content = "\r\n";
	content += request_type;
}
std::string response::get_response(std::string content_str) {
	std::string response = "HTTP/1.1 " + status_code + " " + status_message + "\r\n";
	content += content_str;
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

// response process_request(void) {
// 	// response resp("GET");
// 	return response("POST");
//     // std::string response = "HTTP/1.1 200 OK\r\n";
//     // response += "Content-Type: text/plain\r\n";
//     // response += "Content-Length: 41\r\n";
//     // response += "\r\n";
//     // response += "Hello, world! will i need to change this?";
//     // return response;
// }

response::~response() { }
