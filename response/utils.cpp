/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchliyah <mchliyah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 14:33:29 by mchliyah          #+#    #+#             */
/*   Updated: 2023/04/05 06:17:10 by mchliyah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"
#include <unordered_map>

bool default_index(response &res, client &client, locationconfig& loc, std::string& path)
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
				client.openFile(res, file_path);
				client.setFirstTime(false);
			}
			else 
				client.readFile(res);
			return true;
		}
	}
	return false;
}

bool is_dir(std::string& path)
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

bool is_file(std::string& path)
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
		// std::cout << "frst : "<< frst << std::endl; 
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

std::string get_type(const std::string& path) {
	if (path.find_last_of('.') == std::string::npos) {
		return "application/octet-stream";
	}
    std::unordered_map<std::string, std::string> extension_to_type ;
	extension_to_type[".html"] = "text/html";
	extension_to_type[".txt"] = "text/plain";
	extension_to_type[".css"] = "text/css";
	extension_to_type[".js"] = "application/javascript";
	extension_to_type[".png"] = "image/png";
	extension_to_type[".jpg"] = "image/jpeg";
	extension_to_type[".jpeg"] = "image/jpeg";
	extension_to_type[".gif"] = "image/gif";
	extension_to_type[".svg"] = "image/svg+xml";
	extension_to_type[".ico"] = "image/x-icon";
	extension_to_type[".pdf"] = "application/pdf";
	extension_to_type[".zip"] = "application/zip";
	extension_to_type[".gz"] = "application/gzip";
	extension_to_type[".tar"] = "application/x-tar";
	extension_to_type[".rar"] = "application/x-rar-compressed";
	extension_to_type[".7z"] = "application/x-7z-compressed";
	extension_to_type[".doc"] = "application/msword";
	extension_to_type[".xls"] = "application/vnd.ms-excel";
	extension_to_type[".ppt"] = "application/vnd.ms-powerpoint";
	extension_to_type[".mp3"] = "audio/mpeg";
	extension_to_type[".wav"] = "audio/x-wav";
	extension_to_type[".avi"] = "video/x-msvideo";
	extension_to_type[".mp4"] = "video/mp4";
	extension_to_type[".mpeg"] = "video/mpeg";
	extension_to_type[".mpg"] = "video/mpeg";
	extension_to_type[".mov"] = "video/quicktime";
	extension_to_type[".wmv"] = "video/x-ms-wmv";
	extension_to_type[".flv"] = "video/x-flv";
	extension_to_type[".mkv"] = "video/x-matroska";
	extension_to_type[".webm"] = "video/webm";
	extension_to_type[".ogg"] = "video/ogg";
	extension_to_type[".ogv"] = "video/ogg";
	extension_to_type[".ogm"] = "video/ogg";
	extension_to_type[".ogx"] = "application/ogg";
	extension_to_type[".map"] = "application/json";
	extension_to_type[".cpp"] = "text/plain";
	extension_to_type[".hpp"] = "text/plain";
	extension_to_type[".c"] = "text/plain";
	extension_to_type[".h"] = "text/plain";
	extension_to_type[".conf"] = "text/plain";
	extension_to_type[".json"] = "application/json";
	extension_to_type[".dmg"] = "application/octet-stream";
    std::string extension = path.substr(path.find_last_of('.'));
    std::unordered_map<std::string, std::string>::iterator it = extension_to_type.find(extension);
    if (it != extension_to_type.end()) {
        return it->second;
    } else {
        return "application/octet-stream";
    }
}