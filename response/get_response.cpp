
#include "../includes/server.hpp"

void response::get_response(client& client) {
	try {
	serverconfig server = client.getHost(); 
	std::string in_path = client.getValue("URL");
	std::string name;
	locationconfig location;
	location = matchlocation(server , in_path);
	std::string path = in_path.substr(location.getName().length() - 1, in_path.length() - location.getName().length() + 1);
	std::string file_path = location.getRoot() + path;
	std::string rederec_header = "";
	std::stringstream stream;
	if (location.getAllowsMethod()["GET"] == false) {
		status_code = "405";
		client.errorResponse(*this);
	}
	else if (location.getRedirect() != "")
		return redirect(client, location.getRedirect());
	else if (access(file_path.c_str(), F_OK) != -1)
	{
		if (access(file_path.c_str(), R_OK) != -1)
		{
			if (is_dir(file_path))
			{
				if (in_path[in_path.length() - 1] != '/')
					return redirect(client, in_path);
				if (!default_index(*this, client, location, path))
				{
					if (location.getAutoIndex() == "off" || client.getValue("Method") == "POST")
					{
						status_code = "403";
						client.errorResponse(*this);
					}
					else if (!list_dir(client, file_path)) {
						status_code = "500";
						client.errorResponse(*this);
					}
				}
			}
			else if (is_file(file_path))
			{
				std::string extension = "";
				if(file_path.find('.') != std::string::npos)
					extension = file_path.substr(file_path.find_last_of('.'));
				if ((extension == ".php" || extension == ".py") && location.getCgiPass() == "on")
					client.cgi_response(*this, file_path, extension == ".php");
				else if (client.getValue("Method") == "POST")
				{
					status_code = "403";
					client.errorResponse(*this);
				}
				else
				{
					if (client.getFirstTime())
					{
						client.openFile(*this, file_path);
						client.setFirstTime(false);
					} 
					else
						client.readFile(*this);
				}
			}
		}
		else
		{
			status_code = "403";
			client.errorResponse(*this);
		}
	}
	else
	{
		status_code = "404";
		client.errorResponse(*this);
	}
	client.setBuff(header + body);
	}
	catch (std::exception &e) {
		std::cout << "exception in get response " << std::endl;
	}

}

void response::redirect(client &client, std::string &in_path) {
	std::stringstream stream;
	if (in_path[in_path.length() - 1] != '/')
		in_path += "/";
    status_code = "301";
	status_message = "Moved Permanently";
	body = "";
	content_type = "Content-Type: text/html\r\n";
	stream << body.length();
	content_length = "Content-Length: " + stream.str() + "\r\n";
	header = "HTTP/1.1 " + status_code + " " + status_message + "\r\n"
		+ date + content_type + content_length + "Location: " + in_path + "\r\n";
	header += "\r\n";
	client.setIsSent(1);
	client.setSentBytes(header.size());
	client.setBuff(header);
	return ;
}

bool response::list_dir(client &client, std::string &path) {
	std::stringstream stream;
	DIR *dir;
	struct dirent *ent;
	if (path[path.length() - 1] != '/')
		path += "/";
	if ((dir = opendir (path.c_str())) != NULL) {
		body += "<html><head><title>Index of " + path + "</title></head><body bgcolor=\"white\"><h1>Index of "
				+ path + "</h1><hr><ul><li><a href=\"../\">../</a></li>";

		while ((ent = readdir (dir)) != NULL) {
			if (ent->d_name[0] != '.')
				body += "<li><a href=\"" + std::string(ent->d_name) + "\">" + std::string(ent->d_name) + "</a></li>";
		}
		body += "</ul><hr></body></html>";
		closedir (dir);
		content_type = "Content-Type: text/html\r\n";
		stream << body.length();
		content_length = "Content-Length: " + stream.str() + "\r\n";
		header = "HTTP/1.1 " + status_code + " " + status_message + "\r\n";
		header += date +  content_type + content_length + "\r\n";
		client.setSentBytes(body.size() + header.size());
		client.setIsSent(1);
		return true;
	}
	return false;
}
