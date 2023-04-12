

#include "../includes/server.hpp"

void response::post_response(client& client) {
	serverconfig server = client.getHost();
	std::string in_path = client.getValue("URL");
	locationconfig location;
	location = matchlocation(server, in_path);
	std::string path = location.getUploadStore();
	std::string full_path;
	if (location.getAllowsMethod()["POST"] == false) {
		status_code = "405";
		client.errorResponse(*this);
	}
	else if (path != "" && client.getValue("body") != "multipart")
	{
		if (path[path.length() - 1] != '/')
			path += '/';
		full_path = path + client.getBodyname();
		size_t pos = path.find_first_of(&path[1], '/');
		while (pos < path.length())
		{
			std::string tmp = path.substr(0, pos);
			if (access(tmp.c_str(), F_OK) != 0)
				mkdir(tmp.c_str(), 0777);
			pos = path.find_first_of(&path[pos + 1], '/');
		}
		if (std::rename(client.getBodyname().c_str(), full_path.c_str()) != 0)
		{
			status_code = "500";
			status_message = "Internal Server Error";
			std::remove(client.getBodyname().c_str());
		}
		header = "HTTP/1.1 " + status_code + " " + status_message + "\r\n";
		content_length = "Content-Length: 0\r\n";
		header += date + content_type + content_length;
		if (status_code == "201")
			header += "Location: " + full_path + "\r\n";
		header += "\r\n";
		client.setBuff(header);
		client.setIsSent(1);
		client.setSentBytes(header.size());
	}
	else if (path != "" && client.getValue("body") == "multipart")
	{
		std::vector<std::string>::iterator it;
		std::string ext_header;
		if (path[path.length() - 1] != '/')
			path += '/';
		for (it = client.getMultipart().begin(); it != client.getMultipart().end(); it++)
		{
			full_path = path + *it;
			size_t pos = path.find_first_of(&path[1], '/');
			while (pos < path.length())
			{
				std::string tmp = path.substr(0, pos);
				if (access(tmp.c_str(), F_OK) != 0)
				{
					mkdir(tmp.c_str(), 0777);
				}
				pos = path.find_first_of(&path[pos + 1], '/');
			}
			if (std::rename((*it).c_str(), full_path.c_str()) != 0)
			{
				status_code = "500";
				status_message = "Internal Server Error";
				std::remove((*it).c_str());
				break;
			}
			ext_header += "Location: " + full_path + "\r\n";
		}
		header = "HTTP/1.1 " + status_code + " " + status_message + "\r\n";
		content_length = "Content-Length: 0\r\n";
		header += date + content_type + content_length;
		if (status_code == "201")
			header += ext_header;
		header += "\r\n";
		client.setBuff(header);
		client.setIsSent(1);
		client.setSentBytes(header.size());
	}
	else
		get_response(client);
}