

#include "../includes/server.hpp"

void response::post_response(client& client) {

	header = "HTTP/1.1 " + status_code + " " + status_message + "\r\n";
	body = "";
	content_length += std::to_string(body.size()) + "\r\n";
	header += date + content_type + content_length;
	client.setIsSent(true);
	client.setSentBytes(header.size());
	client.setBuff(header);
}