/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_res.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchliyah <mchliyah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/06 00:19:53 by mchliyah          #+#    #+#             */
/*   Updated: 2023/04/06 09:36:46 by mchliyah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

bool client::openFile(response &res, std::string& path)
{
	if (file.is_open())
		file.close();
	file.open(path.c_str(), std::ios::in | std::ios::binary);
	struct stat buf;
	std::stringstream stream;
	if (file.is_open() && stat(path.c_str(), &buf) == 0) {
		stream << buf.st_size;
		res.set_content_length("Content-Length: " + stream.str() + "\r\n");
		res.set_content_type("Content-Type: "+ get_type(path) + " \r\n");
		res.set_header("HTTP/1.1 " + res.get_status_code() + " " + res.get_status_message() + "\r\n"
			+ res.get_date() +  res.get_content_type() + res.get_content_length() + "\r\n");
		// std::cout << res.get_content_length() << std::endl;
		sent_bytes = res.get_header().size();
	} else
		return (false);
	return (true);
}

bool client::readFile(response &res)
{
	std::vector<char> buff(BUF_SIZE);
	if (!file.eof() && file.is_open()) {
		res.set_body("");
		file.read(&buff[0], BUF_SIZE);
		if (file.gcount() == 0)
			return (false);
		res.set_body(std::string(buff.begin(), buff.begin() + file.gcount()));
		sent_bytes = file.gcount();
	} else  {
		setFirstTime(true);
		setIsSent(1);
		file.close();
		res.get_body().clear();
		return (false);
	} 
	return (true);
}

void client::errorResponse(response &res)
{
	res.set_status_message(res.get_status_code_map()[res.get_status_code()]);
	std::map<std::string, std::string>::iterator it = host.getErrorPages().find(res.get_status_code());
	if (it != host.getErrorPages().end() && access(it->second.c_str(), F_OK) != -1) {
		if (first_time) {
			if (!openFile(res, it->second))
				defaultResponse(res);
			first_time = false;
		} else readFile(res);
	} else defaultResponse(res);
}

void client::defaultResponse(response &res)
{
	std::stringstream stream;
	res.set_content_type("Content-Type: text/html \r\n");
	res.set_body(getHost().getDefaultPage()[res.get_status_code()]);
	stream << res.get_body().size();
	res.set_content_length("Content-Length: " + stream.str() + "\r\n");
	res.set_header("HTTP/1.1 " + res.get_status_code() + " " + res.get_status_message() + "\r\n"
		+ res.get_date() + res.get_content_type() + res.get_content_length() + "\r\n");
	setBuff(res.get_header() + res.get_body());
	sent_bytes = res.get_header().size() + res.get_body().size();
	isSent = 1;
}
