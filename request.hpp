/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 06:23:43 by slahrach          #+#    #+#             */
/*   Updated: 2023/02/28 09:14:05 by slahrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP
#include <map>
#include <string>
#include <algorithm>
#include "socket.hpp"
class request
{
private:
	std::string	_request;
	std::map<std::string, std::string> http_request;
public:
	request(std::string& request);
	void parseRequestLine(std::string first_line);
	void parseHeader(std::string header);
	void parseBody(std::string request);
	void print_attr() const;
	void parse();
	~request();
};
#endif
