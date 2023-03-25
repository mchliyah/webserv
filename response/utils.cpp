/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 14:33:29 by mchliyah          #+#    #+#             */
/*   Updated: 2023/03/25 16:26:45 by slahrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

bool default_index(response &res, locationconfig loc, std::string path)
{
	std::vector<std::string> index = loc.getIndex();
	std::vector<std::string>::iterator it;
	if (path.find_last_of('/') != path.length() - 1)
		path += '/';
	for (it = index.begin() ; it != index.end() ; it++) {
		std::string file_path = loc.getRoot() + path + *it;
		if (access(file_path.c_str(), F_OK) != -1)
		{
			std::ifstream file(file_path);
			if (!file.is_open()){
				res.set_content("403 Forbidden");
				res.set_status_code("403");
				res.set_status_message("Forbidden");
				res.set_content_type("Content-Type: text/html\r\n");
				return true;
			}
			std::string line;
			while (getline(file, line)) {
				res.set_content(res.get_content() + line);
			}
			file.close();
			res.set_content_type("Content-Type: text/html\r\n");
			return true;
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