
#include "../includes/server.hpp"

std::string response::get_response(client& client) {
	serverconfig server = client.getHost(); 
	std::string in_path = client.getValue("URL");
	locationconfig location;
	location = matchlocation(server , in_path);
	std::string path = in_path.substr(location.getName().length() - 1, in_path.length() - location.getName().length() + 1);
	std::string file_path = location.getRoot() + path;
	std::cout << "location name :" << location.getName() << std::endl;
	std::cout << "file path :" << file_path << std::endl;
	if (access(file_path.c_str(), F_OK) != -1)
	{
		if (is_dir(file_path))
		{
			if (file_path[file_path.length() - 1] != '/')
			{
				std::cout << "redirect" << std::endl;
				in_path += "/";
				std::cout << "in_path "<< in_path << std::endl;
    			status_code = "301";
				header = "HTTP/1.1 " + status_code + " " + status_message + "\r\n";
				header += "Date: " + date + "\r\n";
				header += "Location: " + in_path + "\r\n";
				header += "Content-Type: text/html\r\n";
				header += "Content-Length: 0\r\n";
				header += "\r\n";
				body = "";
				client.setIsSent(1);
    			return (put_response());
			}
			if (!default_index(*this, client, location, path) && location.getAutoIndex() == "on")
			{
				DIR *dir;
				struct dirent *ent;
				if (file_path[file_path.length() - 1] != '/')
					file_path += "/";
				if ((dir = opendir (file_path.c_str())) != NULL)
				{
					header += "<html><head><title>Index of " + path + "</title></head><body bgcolor=\"white\"><h1>Index of "
						+ path + "</h1><hr><pre><a href=\"../\">../</a>";
					while ((ent = readdir (dir)) != NULL)
					{
						if (ent->d_name[0] != '.')
							header += " <a href=\"" + std::string(ent->d_name) + "\">" + std::string(ent->d_name) + "</a>";
					}
					header += "</pre><hr></body></html>";
					closedir (dir);
					content_type = "Content-Type: text/html\r\n";
					client.setIsSent(1);
					return (put_response());
				}
			}
			// else
			// {
			// 	std::cout << "no permission or autoindex off" << std::endl;
			// 	status_code = "403";
			// 	status_message = "Forbidden";
			// 	content = "HTTP/1.1 " + status_code + " " + status_message + "\r\n";
			// 	body = "<!DOCTYPE html><html><head>403 Forbidden</head><body><p>no permission or autoindex off</p></body></html>";
			// 	content_type = "Content-Type: text/html\r\n";
			// 	client.setIsSent(1);
			// }
		}
		else if (is_file(file_path))
		{
			std::cout << "file" << std::endl;
			if (client.getFirstTime()) {
				client.openFile(*this, file_path);
				client.setFirstTime(false);
			}
			else
				client.readFile(*this);
		}
		else
		{
			std::cout << "no permission" << std::endl;
			status_code = "403";
			status_message = "Forbidden";
			header = "HTTP/1.1 " + status_code + " " + status_message + "\r\n";
			body = "<!DOCTYPE html><html><head>403 Forbidden</head><body><p>no permission</p></body></html>";
			content_type = "Content-Type: text/html\r\n";
			client.setIsSent(1);
		}
	}
	else
	{
		std::cout << "no file or directory" << std::endl;
		status_code = "404";
		status_message = "Not Found";
		header = "HTTP/1.1 " + status_code + " " + status_message + "\r\n";
		body = "<!DOCTYPE html><html><head>404 Not Found</head><body><p>no file or directory</p></body></html>";
		content_type = "Content-Type: text/html\r\n";
		client.setIsSent(1);
	}
	return put_response();
}
