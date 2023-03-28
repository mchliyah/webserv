/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchliyah <mchliyah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 14:33:29 by mchliyah          #+#    #+#             */
/*   Updated: 2023/03/28 00:18:13 by mchliyah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

bool default_index(response &res, client &client, locationconfig loc, std::string path)
{
	std::vector<std::string> index = loc.getIndex();
	std::vector<std::string>::iterator it;
	if (path.find_last_of('/') != path.length() - 1)
		path += '/';
	for (it = index.begin() ; it != index.end() ; it++) {
		std::string file_path = loc.getRoot() + path + *it;
		if (access(file_path.c_str(), F_OK) != -1)
		{
			if (client.getFirstTime())
			{
				std::cout << "First time after index found " << std::endl;
				client.setReadfds(open(file_path.c_str(), O_RDONLY));
				struct stat buf;
				if (stat(file_path.c_str(), &buf) == 0)
				{
					res.set_content_length("Content-Length: " + std::to_string(buf.st_size) + "\r\n");
					res.set_status_code("200");
					res.set_status_message("OK");
					res.set_content_type("Content-Type: text/html\r\n");
					res.set_content("HTTP/1.1 200 OK \r\n"
						+ res.get_date() + res.get_content_type() + res.get_content_length() + "\r\n");
					std::vector<std::string>::iterator it;
					for (it = res.get_headers().begin(); it != res.get_headers().end(); it++)
						res.set_content(res.get_content() + *it);
					res.set_content(res.get_content() + "\r\n");
					res.set_body("");
				}
				else
				{
					res.set_status_code("500");
					res.set_status_message("Internal Server Error");
					res.set_content_type("Content-Type: text/html\r\n");
					res.set_body("<!DOCTYPE html><html><head>500 Internal Server Error</head><body><p>stat error</p></body></html>");
					res.set_content_length("Content-Length: " + std::to_string(res.get_body().length()) + "\r\n");
					res.set_content("http/1.1 500 Internal Server Error\r\n"
						+ res.get_date() + res.get_content_type() + res.get_content_length() + "\r\n");
					std::vector<std::string>::iterator it;
					for (it = res.get_headers().begin(); it != res.get_headers().end(); it++)
						res.set_content(res.get_content() + *it);
					res.set_content(res.get_content() + "\r\n");
					client.setReadfds(-1);
					client.setIsSent(1);
				}
				client.setFirstTime(false);
				return true;
			}
			else if (client.getReadfds() != -1)
			{
				char buf[1024];
				int fd = client.getReadfds();
				int ret = read(fd, buf, 1024);
				res.set_content("");
				if (ret <= 0 )
				{
					std::cout << "read error after index found " << std::endl;
					close(fd);
					client.setReadfds(-1);
					client.setFirstTime(true);
					client.setIsSent(1);
				}
				else
				{
					std::cout << "read success after index found " << std::endl;
					res.set_body(std::string(buf, ret));
				}
				return true;
			}
		}
	}
	return false;
}

bool is_dir(std::string path)
{
	struct stat s;
	if (stat(path.c_str(), &s) == 0)
	{
		if (s.st_mode & S_IFDIR)
			return true;
		else if (s.st_mode & S_IFREG)
			return false;
	}
	return false;
}

bool is_file(std::string path)
{
	struct stat s;
	if (stat(path.c_str(), &s) == 0)
	{
		if (s.st_mode & S_IFREG)
			return true;
		else if (s.st_mode & S_IFDIR)
			return false;
	}
	return false;
}

locationconfig matchlocation(serverconfig &server, std::string &path)
{
	locationconfig location;
	bool found = false;
	std::map<std::string, locationconfig> locations = server.getLocations();
	std::map<std::string, locationconfig>::iterator loc;
	if (path.empty())
		path = "/";
	std::string frst = path;
	while (frst.length() > 1 && !found)
	{
		std::cout << "frst : "<< frst << std::endl; 
		for (loc = locations.begin() ; loc != locations.end() ; loc++)
		{
			if (loc->first.compare(frst + "/") == 0)
			{
				location = loc->second;
				found = true;
				break;
			}
		}
		std::string::size_type pos = frst.find_last_of('/');
		if (pos != std::string::npos)
			frst = frst.substr(0, pos);
	}
	if (!found || path == "/")
		location = locations.begin()->second;
	return (location);
}