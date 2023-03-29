/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 16:51:42 by mchliyah          #+#    #+#             */
/*   Updated: 2023/03/29 06:53:57 by slahrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

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
	body = "";
	content_type = "Content-Type: text/plain\r\n";
	content_length = "Content-Length: ";
}

std::string response::put_response(void)
{
	std::string response;
	response += header;
	response += body;
	response += "\r\n";
	std::cout << "==========================================Response: " << std::endl;
	std::cout << response << std::endl;
	std::cout << "==========================================" << std::endl;
	return response;
}

std::string response::get_status_code() { return status_code; }

std::string response::get_status_message() { return status_message; }

std::string response::get_content_type() { return content_type; }

std::string response::get_content() { return header; }

std::string response::get_content_length() { return content_length; }

std::string response::get_date() { return date; }

std::vector<std::string> response::get_headers() { return headers; }

std::string response::get_body(void ) const { return body; }

void response::set_body(std::string body) { this->body = body; }

void response::set_header(std::string content) { this->header = content; }

void response::set_content_type(std::string content_type) { this->content_type = content_type; }

void response::set_content_length(std::string content_length) { this->content_length = content_length; }

void response::set_status_code(std::string status_code) { this->status_code = status_code; }

void response::set_status_message(std::string status_message) { this->status_message = status_message; }

void response::add_to_header(std::string header) { this->header += header ; }

void response::clear_header(void) { this->header = ""; }

std::string response::get_header(void) const { return (this->header); }


response::~response() { }
