

#include "../includes/server.hpp"

void response::delete_response(client& client) {

	serverconfig server = client.getHost();
	locationconfig location;
	std::string in_path = client.getValue("URL");
	location = matchlocation(server , in_path);
	std::string path = in_path.substr(location.getName().length() - 1, in_path.length() - location.getName().length() + 1);
	std::string full_path = location.getRoot() + path;
	std::cout << "full_path : " << full_path << std::endl;
	if (remove(full_path.c_str()) != 0)
	{
		status_code = "500";
		status_message = "Internal Server Error";
		std::cout << "Error deleting file" << std::endl;
	}
	header = "HTTP/1.1 " + status_code + " " + status_message + "\r\n";
	content_length += "Content-Length: 0\r\n";
	header += date + content_type + content_length;
	client.setIsSent(true);
	client.setSentBytes(header.size());
	client.setBuff(header);
}