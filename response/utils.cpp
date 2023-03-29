/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchliyah <mchliyah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 14:33:29 by mchliyah          #+#    #+#             */
/*   Updated: 2023/03/28 21:19:49 by mchliyah         ###   ########.fr       */
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
				client.openFile(res, file_path);
				client.setFirstTime(false);
			}
			else 
				client.readFile(res);
			return true;
		}
	}
	std::cout << "No index found" << std::endl;
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