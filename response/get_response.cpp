
#include "../includes/server.hpp"

void response::get_response(client& client) {
	serverconfig server = client.getHost(); 
	std::string in_path = client.getValue("URL");
	std::string name;
	locationconfig location;
	location = matchlocation(server , in_path);
	std::string path = in_path.substr(location.getName().length() - 1, in_path.length() - location.getName().length() + 1);
	std::string file_path = location.getRoot() + path;
	// std::cout << "location name :" << location.getName() << std::endl;
	// std::cout << "file path :" << file_path << std::endl;
	if (client.getValue("body") == "present")
	{
		std::remove(client.getBodyname().c_str());
		status_code = "400";
		status_message = "Bad Request";
		body = "";
		content_type = "Content-Type: text/html\r\n";
		content_length = "Content-Length: " + std::to_string(body.length()) + "\r\n";
		header = "HTTP/1.1 " + status_code + " " + status_message + "\r\n"
			+ date + content_type + content_length + "\r\n";
		client.setIsSent(1);
		client.setSentBytes(header.size());
		client.setBuff(header);
		return ;
	}
	if (access(file_path.c_str(), F_OK) != -1)
	{
		if (is_dir(file_path))
		{
			if (file_path[file_path.length() - 1] != '/')
			{
				in_path += "/";
    			status_code = "301";
				status_message = "Moved Permanently";
				body = "";
				content_type = "Content-Type: text/html\r\n";
				content_length = "Content-Length: " + std::to_string(body.length()) + "\r\n";
				header = "HTTP/1.1 " + status_code + " " + status_message + "\r\n"
					+ date + content_type + content_length + "\r\n" + "Location: " + in_path + "\r\n";
				client.setIsSent(1);
				client.setSentBytes(header.size());
				client.setBuff(header);
				return ;
			}
			if (!default_index(*this, client, location, path) && location.getAutoIndex() == "on")
			{
				DIR *dir;
				struct dirent *ent;
				if (file_path[file_path.length() - 1] != '/')
					file_path += "/";
				status_code = "200";
				status_message = "OK";
				if ((dir = opendir (file_path.c_str())) != NULL)
				{
					body += "<html><head><title>Index of " + path + "</title></head><body bgcolor=\"white\"><h1>Index of "
        					+ path + "</h1><hr><ul><li><a href=\"../\">../</a></li>";

					while ((ent = readdir (dir)) != NULL)
					{
					    if (ent->d_name[0] != '.')
					        body += "<li><a href=\"" + std::string(ent->d_name) + "\">" + std::string(ent->d_name) + "</a></li>";
					}
					body += "</ul><hr></body></html>";
					closedir (dir);
					content_type = "Content-Type: text/html\r\n";
					content_length = "Content-Length: " + std::to_string(body.length()) + "\r\n";
					header = "HTTP/1.1 " + status_code + " " + status_message + "\r\n";
					header += date +  content_type + content_length + "\r\n";
					client.setSentBytes(body.size() + header.size());
					client.setIsSent(1);
				}
				else
				{
					status_code = "403";
					status_message = "Forbidden";
					content_type = "Content-Type: text/html\r\n";
					body = "<!DOCTYPE html><html><head>403 Forbidden</head><body><p>no permission</p></body></html>";
					content_length = "Content-Length: " + std::to_string(body.size()) + "\r\n"; 
					header = "HTTP/1.1 " + status_code + " " + status_message + "\r\n";
					header += date + content_type + content_length + "\r\n";
					client.setSentBytes(header.size() + body.size());
					client.setIsSent(1);
				}
			}
		}
		else if (is_file(file_path))
		{
			// if (location.getCgiPass().size() > 0 && location.getCgiPass().find(path) != std::string::npos)
			// {
			// 	cgi_response(client);
			// 	return ;
			// }
			// std::cout << "file" << std::endl;
			if (client.getFirstTime()) {
				client.openFile(*this, file_path);
				client.setFirstTime(false);
			}
			else
				client.readFile(*this);
		}
		else
		{
			status_code = "403";
			status_message = "Forbidden";
			content_type = "Content-Type: text/html\r\n";
			body = "<!DOCTYPE html><html><head>403 Forbidden</head><body><p>no permission</p></body></html>";
			content_length = "Content-Length: " + std::to_string(body.size()) + "\r\n"; 
			header = "HTTP/1.1 " + status_code + " " + status_message + "\r\n";
			header += date + content_type + content_length + "\r\n";
			client.setSentBytes(header.size() + body.size());
			client.setIsSent(1);
		}
	}
	else
	{
		status_code = "404";
		status_message = "Not Found";
		content_type = "Content-Type: text/html\r\n";
		body = "<!DOCTYPE html><html><head>404 Not Found</head><body><p>no file or directory</p></body></html>";
		content_length = "Content-Length: " + std::to_string(body.size()) + "\r\n";
		header = "HTTP/1.1 " + status_code + " " + status_message + "\r\n";
		header += date + content_type + content_length + "\r\n";
		client.setSentBytes(header.size() + body.size());
		client.setIsSent(1);
	}
	client.setBuff(header + body);
}
