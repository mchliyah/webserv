

#include "../includes/server.hpp"

void response::post_response(client& client) {
	serverconfig server = client.getHost();
	std::string in_path = client.getValue("URL");
	locationconfig location;
	location = matchlocation(client.getHost(), in_path);
	std::string path = in_path.substr(location.getName().length() - 1, in_path.length() - location.getName().length() + 1);
	std::string full_path = location.getRoot() + path;
	std::cout << "path name " << full_path << std::endl;
	if (access(full_path.c_str(), F_OK) != -1) 
	{
		status_code = "405";
		status_message = "Method Not Allowed";
	}
	else if (client.getValue("body") == "")
	{
		status_code = "411";
		status_message = "body is empty";
	}
	else if (std::rename(client.getBodyname().c_str(), full_path.c_str()) != 0)
	{
		status_code = "500";
		status_message = "Internal Server Error";
	}
	header = "HTTP/1.1 " + status_code + " " + status_message + "\r\n";
	content_length = "Content-Length: 0\r\n";
	header += date + content_type + content_length + "\r\n";
	client.setBuff(header);
	client.setIsSent(1);
	client.setSentBytes(header.size());
	std::remove(client.getBodyname().c_str());
}