/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchliyah <mchliyah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 16:51:42 by mchliyah          #+#    #+#             */
/*   Updated: 2023/03/15 20:32:48 by mchliyah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

response::response(void) {
	this->status = "HTTP/1.1 200 OK\r\n";
	this->content_type = "Content-Type: text/plain\r\n";
	this->content_length = "Content-Length: 41\r\n";
	this->content = "\r\nHello, world! will i need to change this?";
	this->resp = this->status + this->content_type + this->content_length + this->content;
}


response process_request(void) {
	return response();
    // std::string response = "HTTP/1.1 200 OK\r\n";
    // response += "Content-Type: text/plain\r\n";
    // response += "Content-Length: 41\r\n";
    // response += "\r\n";
    // response += "Hello, world! will i need to change this?";
    // return response;
}

response::~response() { }

std::string response::get_response() { return this->resp; }

std::string response::get_status() { return this->status; }

std::string response::get_content_type() { return this->content_type; }

std::string response::get_content_length() { return this->content_length; }

std::string response::get_content() { return this->content; }

std::string response::get_date() { return this->date; }

std::string response::get_location() { return this->location; }

std::vector<std::string> response::get_headers() { return this->headers; }

std::vector<std::string> response::get_body() { return this->body; }

void response::set_response(std::string resp) { this->resp = resp; }

void response::set_status(std::string status) { this->status = status; }

void response::set_content_type(std::string content_type) { this->content_type = content_type; }

void response::set_content_length(std::string content_length) { this->content_length = content_length; }

void response::set_content(std::string content) { this->content = content; }

void response::set_date(std::string date) { this->date = date; }

void response::set_location(std::string location) { this->location = location; }

void response::set_headers(std::vector<std::string> headers) { this->headers = headers; }

void response::set_body(std::vector<std::string> body) { this->body = body; }

// Path: response/response.cpp


