

#include "../includes/server.hpp"

bool delete_directory(const std::string& path) {
    DIR* dir = opendir(path.c_str());
    if (dir == NULL) {
        return false;
    }
    dirent* entry = readdir(dir);
    while (entry != NULL) {
        std::string file_name = entry->d_name;
        if (file_name != "." && file_name != "..") {
            std::string full_path = path + "/" + file_name;
			if (access(full_path.c_str(), W_OK) != -1)
			{
            	if (is_dir(full_path)) {
            	    if (!delete_directory(full_path)) {
            	        closedir(dir);
            	        return false;
            	    }
            	} else {
            	    if (remove(full_path.c_str()) != 0) {
            	        closedir(dir);
            	        return false;
            	    }
            	}
			}
        }
        entry = readdir(dir);
    }
    closedir(dir);
    if (rmdir(path.c_str()) != 0) {
        return false;
    }
    return true;
}

void response::delete_response(client& client) {

	serverconfig server = client.getHost();
	locationconfig location;
	std::string in_path = client.getValue("URL");
	location = matchlocation(server , in_path);
	std::string path = in_path.substr(location.getName().length() - 1, in_path.length() - location.getName().length() + 1);
	std::string full_path = location.getRoot() + path;
	if (full_path[full_path.length() - 1] == '/')
		full_path = full_path.substr(0, full_path.length() - 1);
	status_code = "204";
	status_message = "No Content";
	if (location.getAllowsMethod()["DELETE"] == false) {
		status_code = "405";
		client.errorResponse(*this);
	}
	else if (access(full_path.c_str(), F_OK) != -1)
	{
		if (access(full_path.c_str(), W_OK) != -1)
		{
			if ((is_dir(full_path) && delete_directory(full_path)) ||
				(is_file(full_path) && remove(full_path.c_str()) == 0))
				{
					status_code = "204";
					status_message = "No Content";
					header = "HTTP/1.1 " + status_code + " " + status_message + "\r\n";
					content_length += "Content-Length: 0\r\n";
					header += date + content_type + content_length + "\r\n";
					client.setIsSent(true);
					client.setSentBytes(header.size());
					client.setBuff(header);
					return ;
				}
				else
				{
					status_code = "500";
					client.errorResponse(*this);
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