/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchliyah <mchliyah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/15 16:51:45 by mchliyah          #+#    #+#             */
/*   Updated: 2023/04/10 01:36:29 by mchliyah         ###   ########.fr       */
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
		std::map<std::string, std::string> status_code_map;
	public:
		response();
		response(const std::string& request_type);
		response(const response &src);
		response &operator=(const response &src);
		~response();
		std::string get_status_code();
		std::string get_status_message();
		std::string get_content_type();
		std::string get_content();
		std::string get_content_length();
		std::string get_date();
		void set_header(std::string header);
		void set_content_type(std::string content_type);
		void set_content_length(std::string content_length);
		void set_status_code(std::string status_code);
		void set_status_message(std::string status_message);
		void set_body(std::string body);
		void add_to_header(std::string header);
		std::string get_header(void) const;
		void clear_header(void);
		std::string get_body(void);
		void get_response(client &client);
		void post_response(client& client);
		void delete_response(client& client);
		std::map<std::string, std::string> get_status_code_map();
		void redirect(client& client, std::string& in_path);
		bool list_dir(client& client, std::string& path);
		void clear();
		void clearall();
};

bool is_dir(std::string& path);
bool is_file(std::string& path);
bool default_index(response &res, client &client, locationconfig& loc, std::string& path);
locationconfig matchlocation(serverconfig &server, std::string &path);

#endif