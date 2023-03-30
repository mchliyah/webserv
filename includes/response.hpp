/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchliyah <mchliyah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 16:51:45 by mchliyah          #+#    #+#             */
/*   Updated: 2023/03/30 08:52:24 by mchliyah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef RESPONSE_HPP
# define RESPONSE_HPP

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
#include "../includes/client.hpp"

class client;

class response
{
	private:
		std::string status_code;
    	std::string status_message;
    	std::string content_type;
    	std::string header;
		std::string body;
		std::string content_length;
    	std::string date;
    	std::vector<std::string> headers;
	public:
		response(const std::string& request_type);
		~response();
		std::string get_status_code();
		std::string get_status_message();
		std::string get_content_type();
		std::string get_content();
		std::string get_content_length();
		std::string get_date();
		std::vector<std::string> get_headers();
		void set_header(std::string header);
		void set_content_type(std::string content_type);
		void set_content_length(std::string content_length);
		void set_status_code(std::string status_code);
		void set_status_message(std::string status_message);
		void set_date(std::string date);
		void set_body(std::string body);
		void add_to_header(std::string header);
		std::string get_header(void) const;
		void clear_header(void);
		std::string& get_body(void);
		std::string get_response(client &client);
		std::string post_response(serverconfig &server, std::string &path, std::string &body);
		std::string delete_response(serverconfig &server, std::string &path);
		std::string put_response(void);
};

bool is_dir(std::string path);
bool is_file(std::string path);
bool default_index(response &res, client &client, locationconfig loc, std::string path);
locationconfig matchlocation(serverconfig &server, std::string &path);

#endif