/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchliyah <mchliyah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/24 14:33:29 by mchliyah          #+#    #+#             */
/*   Updated: 2023/03/24 15:50:56 by mchliyah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/server.hpp"

bool default_index(response &res, std::map<std::string, locationconfig>::iterator loc, std::string path)
{
	std::vector<std::string> index = loc->second.getIndex();
	std::vector<std::string>::iterator it;
	if (path.find_last_of('/') != path.length() - 1)
		path += '/';
	for (it = index.begin() ; it != index.end() ; it++) {
		std::string file_path = loc->second.getRoot() + path + *it;
		if (access(file_path.c_str(), F_OK) != -1)
		{
			std::ifstream file(file_path);
			if (!file.is_open()){
				res.set_content("403 Forbidden");
				res.set_status_code("403");
				res.set_status_message("Forbidden");
				res.set_content_type("Content-Type: text/html\r\n");
				return true;
			}
			std::string line;
			while (getline(file, line)) {
				res.set_content(res.get_content() + line);
			}
			file.close();
			res.set_content_type("Content-Type: text/html\r\n");
			return true;
		}
	}
	return false;
}