/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 09:12:48 by slahrach          #+#    #+#             */
/*   Updated: 2023/03/01 08:51:10 by slahrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

request::request(std::string& request) : _request(request) , error(0), err_message("")
{
	this->parse();
	this->errorHandling();
}

void request::makeError(int err, const std::string& msg)
{
	error= err;
	err_message = msg;
}
request::~request() {}
void	request::errorHandling()
{
	bool	found = 0;
	std::string	methods[3] = {"GET", "POST", "DELETE"};
	std::string method = getValue("Method");
	if (method.empty())
		makeError(400, "Bad Request: Method not found");
	for (int i = 0; i < 3; i++)
		if (methods[i] == method)
			found = 1;
	if (!found)
		makeError(405, "Bad Request: Method Not supported");
}

void	print(std::pair<std::string, std::string> p)
{
	std::cout << p.first << " is : " << p.second << std::endl;
}
void request::printAttr() const
{
	for_each(http_request.begin(), http_request.end(), print);
}

void request::parseRequestLine(std::string first_line)
{
	size_t	pos;
	size_t	last;
	std::string items[3] = {"Method", "URL", "Version"};

	for (int i = 0; i < 3; i++)
	{
		pos = first_line.find(" ", last);
		if (pos != std::string::npos)
			http_request[items[i]] = first_line.substr(last, pos - last);
		last = pos + 1;
	}
}
void request::parseHeader(std::string header)
{
	std::string	key = "";
	std::string	value = "";
	size_t		pos;

	pos = header.find(":");
	key = header.substr(0, pos);
	value = header.substr(pos + 1);
	http_request[key] = value;
}
void request::parseBody(std::string body)
{
	http_request["Body"] = body;
}
void request::parse()
{
	std::size_t	pos;
	std::size_t	last;

	
	pos = _request.find_first_of("\r\n");
	if (pos != std::string::npos)
		parseRequestLine(_request.substr(0, pos));
	while (1)
	{
		last = pos + 2;
		pos = _request.find("\r\n", last);
		if (pos != std::string::npos)
		{
			std::string line = _request.substr(last, pos - last);
			if (!line.empty())
				parseHeader(line);
			else
				break ;
		}
	}
	if (pos + 2 != _request.size())
		parseBody(_request.substr(pos + 2));
}
std::string&	request::getValue(const std::string& key)
{
	return (http_request[key]);
}
