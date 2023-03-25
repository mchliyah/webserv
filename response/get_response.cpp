
#include "../includes/server.hpp"

std::string response::get_response(serverconfig &server, std::string &path) {
	locationconfig location;
	location = matchlocation(server , path);
	std::string file_path = location.getRoot() + path;
	if (access(file_path.c_str(), F_OK) != -1)
	{
		if (is_dir(file_path))
		{
			if (!default_index(*this, location, path) && location.getAutoIndex() == "on")
			{
				DIR *dir;
				struct dirent *ent;
				if (file_path[file_path.length() - 1] != '/')
					file_path += "/";
				if ((dir = opendir (file_path.c_str())) != NULL)
				{
					content += "<html><head><title>Index of " + path + "</title></head><body bgcolor=\"white\"><h1>Index of " + path + "</h1><hr><pre><a href=\"../\">../</a>";
					while ((ent = readdir (dir)) != NULL)
					{
						if (ent->d_name[0] != '.')
							content += " <a href=\"" + std::string(ent->d_name) + "\">" + std::string(ent->d_name) + "</a>";
					}
					content += "</pre><hr></body></html>";
					closedir (dir);
					content_type = "Content-Type: text/html\r\n";
				}
			}
		}
		else if (is_file(file_path))
		{
			int fd = open(file_path.c_str(), O_RDONLY);
			if (fd != -1)
			{
				char buf[2048];
				int ret;
				while ((ret = read(fd, buf, 2048)) > 0)
					content.append(buf, ret);
				close(fd);
			}
			content_type = "Content-Type: text/html\r\n";
		}	
	}
	else
	{
		status_code = "404";
		status_message = "Not Found";
		content = "<!DOCTYPE html><html><head>404 Not Found</head><body><p>no file or directory</p></body></html>";
		content_type = "Content-Type: text/html\r\n";
	}
	return put_response();
}
