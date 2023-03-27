/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 09:12:48 by slahrach          #+#    #+#             */
/*   Updated: 2023/03/27 09:50:32 by slahrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/client.hpp"

client::client(int sock, std::string port_) : request("") ,port(port_),socket_fd(sock), isSent(0), error(200), err_message(""), rcv(0)
{
}
client::~client(){}
//getters & setters
int client::getSocket() const {return socket_fd;}
std::string client::getPort() const {return port;}
bool client::getIsSent() const {return isSent;}
void client::setIsSent(bool b) {isSent = b;}
void client::resetRequest(){request = "";}

void client::makeError(int err, const std::string& msg)
{
	error= err;
	err_message = msg;
}
int	client::checkMethod()
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

void	client::printAttr() const
{
	for_each(http_request.begin(), http_request.end(), print);
}

bool BothAreSpaces(char lhs, char rhs) { return (lhs == rhs) && (lhs == ' '); }



int	client::parseRequestLine(std::string first_line)
{
	size_t	pos;
	size_t	last;

	last = 0;
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
void client::parseHeader(std::string header)
{
	std::string	key = "";
	std::string	value = "";
	size_t		pos;

	pos = header.find(":");
	key = header.substr(0, pos);
	if (pos + 1 < header.size() && header[pos + 1] == ' ')
		pos++;
	value = header.substr(pos + 1);
	http_request[key] = value;
}
void client::addToBody(std::string body)//MAKE IT RETURN 
{
	//put body in a file
	std::stringstream stream;
	std::ofstream file;
	stream << socket_fd;
	std::string filename = "body" + stream.str() + ".txt";
	std::cout << "rcv is " << rcv << std::endl;
	if (rcv == 2)
	{
		std::cout << "Nom Im clearing" << std::endl;
		file.open("body" + stream.str() + ".txt", std::ios::out | std::ios::trunc);
		file.close();
	}
	file.open("body" + stream.str() + ".txt", std::ios::app);
	file.seekp(0, std::ios::end);
	std::streampos size = file.tellp();
	// if (!getValue("Transfer-Encoding").empty() && http_request["Transfer-Encoding"] == "chunked")
	// {
	// 	size_t pos = 0;
	// 	size_t old = 0;
	// 	std::cout << "here" << std::endl;
	// 	while (old < body.length())
	// 	{
	// 		pos = body.find("\r\n", old);
	// 		std::cout << old << pos << std::endl;
	// 		std::string length_string = body.substr(old, pos - old);
	// 		long l = strtol(length_string.c_str(), NULL, 16);
	// 		if (l == 0)
	// 			break ;
	// 		final += body.substr(pos+2, l);
	// 		old = pos + 4 + l;
	// 	}
	// }
	std::string length = http_request["Content-Length"];
	std::stringstream l(length);
	int length_int;
	l >> length_int;
	int s = length_int - size;
	std::cout << "now i'm writing -";
	for (int i = 0; i < (int)body.length() && i < s; i++)
	{
		file << body[i];
		std::cout << body[i];
	}
	std::cout << "-" << std::endl;
	file.seekp(0, std::ios::end);
	int si = file.tellp();
	std::cout << "size of file is " << si<< " and content length is " << length_int << std::endl;
	if (rcv != 4)
		rcv = 3;
	if (size == length_int)
		rcv = 4;
	file.close();
}
void client::parse()
{
	std::size_t	pos;
	std::size_t	last;

	
	pos = request.find_first_of("\r\n");
	if (pos == std::string::npos)
	{
		makeError(400, "bad request: Seperator Is Missing");
		return ;
	}
	if (parseRequestLine(request.substr(0, pos)))
		return ;
	while (1)
	{
		last = pos + 2;
		pos = request.find("\r\n", last);
		if (pos == std::string::npos)
		{
			makeError(400, "Bad Request: Seperator Is Missing");
			return ;
		}
		std::string line = request.substr(last, pos - last);
		if (!line.empty())
			parseHeader(line);
		else
			break ;
	}
}
std::string&	client::getValue(const std::string& key)
{
	return (http_request[key]);
}

int client::getError() const {return error;}

std::string client::getErrorMessage() const {return err_message;}

int client::checkMandatoryElements()
{
	if (getValue("Host").empty())
	{
		makeError(400, "Bad Request: Missing Host!");
		return (1);
	}
	if (!getValue("Content-Length").empty())
	{
		std::istringstream iss(getValue("Content-Length"));
		int num;
		iss >> num;
		http_request["Body"] = getValue("Body").erase(num, http_request["Body"].size() - num);
	}
	return (0);
}
void client::matchHost(std::vector<serverconfig> hosts)
{
	std::string name = http_request["Host"];
	size_t f = name.find(":");
	if (f != std::string::npos)
		name = name.substr(0, f);
	host = *(hosts.begin());
	for (std::vector<serverconfig>::iterator it= hosts.begin(); it < hosts.end(); it++)
	{
		if (it->getServerName() == name && it->getListen() == port)
			host = *it;
	}
}

serverconfig client::getHost(void)
{
	return (this->host);
}

std::string	client::addToRequestCheck(std::string buff)
{
	std::string rest = "";
	request += buff;
	if (request.find("\r\n\r\n") != std::string::npos)
	{
		rcv = 1;
		rest = request.substr(request.find("\r\n\r\n") + 4);
		request = request.substr(0, request.find("\r\n\r\n") + 4);
		std::cout << "request : -" << request << "-rest : -" << rest << "-"<< std::endl;
		parse();
	}
	return (rest);
}
