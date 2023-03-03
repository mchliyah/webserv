/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   request.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 09:12:48 by slahrach          #+#    #+#             */
/*   Updated: 2023/03/03 11:18:14 by slahrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "request.hpp"

request::request(std::string& request) : _request(request) , error(0), err_message("")
{
	this->parse();
}

void request::makeError(int err, const std::string& msg)
{
	error= err;
	err_message = msg;
}
request::~request() {}
int	request::checkMethod()
{
	bool	found = 0;
	std::string	methods[3] = {"GET", "POST", "DELETE"};
	std::string method = getValue("Method");
	for (int i = 0; i < 3; i++)
		if (methods[i] == method)
			found = 1;
	if (!found)
	{
		makeError(405, "Bad Request: Method Not supported");
		return (1);
	}
	return (0);
}

void	print(std::pair<std::string, std::string> p)
{
	std::cout << p.first << " is : " << p.second << std::endl;
}

void	request::printAttr() const
{
	for_each(http_request.begin(), http_request.end(), print);
}

bool BothAreSpaces(char lhs, char rhs) { return (lhs == rhs) && (lhs == ' '); }



int	request::parseRequestLine(std::string first_line)
{
	size_t	pos;
	size_t	last;

	std::string items[3] = {"Method", "URL"};
	std::string::iterator new_end = std::unique(first_line.begin(), first_line.end(), BothAreSpaces);
	first_line.erase(new_end, first_line.end());
	if (first_line[0] == ' ')
		first_line.erase(0, 1);
	if (*first_line.rbegin() == ' ')
		first_line.pop_back();
	for (int i = 0; i < 2; i++)
	{
		pos = first_line.find(" ", last);
		if (pos == std::string::npos || (i == 1 && first_line.find(" ", pos + 1) != std::string::npos))
		{
			makeError(400, "Bad Request : Request Line Not Valid");
			return (1);
		}
		http_request[items[i]] = first_line.substr(last, pos - last);
		last = pos + 1;
	}
	http_request["version"] = first_line.substr(pos + 1);
	if (checkMethod())
		return (1);
	return 0;
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
	if (pos == std::string::npos)
	{
		makeError(400, "bad request: Seperator Is Missing");
		return ;
	}
	if (parseRequestLine(_request.substr(0, pos)))
		return ;
	while (1)
	{
		last = pos + 2;
		pos = _request.find("\r\n", last);
		if (pos == std::string::npos)
		{
			makeError(400, "Bad Request: Seperator Is Missing");
			return ;
		}
		std::string line = _request.substr(last, pos - last);
		if (!line.empty())
			parseHeader(line);
		else
			break ;
	}
	if (pos + 2 < _request.size())
		parseBody(_request.substr(pos + 2));
	if (checkMandatoryElements())
		return ;
}
std::string&	request::getValue(const std::string& key)
{
	return (http_request[key]);
}

int request::getError() const {return error;}

std::string request::getErrorMessage() const {return err_message;}

int request::checkMandatoryElements()
{
	if (getValue("Host").empty())
	{
		makeError(400, "Bad Request: Host is Missing");
		return (1);
	}
	//add checks for every method
	return (0);
}
