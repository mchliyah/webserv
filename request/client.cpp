/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchliyah <mchliyah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 09:12:48 by slahrach          #+#    #+#             */
/*   Updated: 2023/03/27 03:50:51 by mchliyah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/client.hpp"

client::client(int sock, std::string port_) : request("") ,port(port_),socket_fd(sock), isSent(0), error(0), first_time(true), readfds(-1), err_message("")
{
	this->parse();
}
client::~client() {}
//getters & setters
int client::getSocket() const {return socket_fd;}
std::string client::getPort() const {return port;}
bool client::getIsSent() const {return isSent;}
void client::setIsSent(bool b) {isSent = b;}
void client::setRequest(char *req) {request = req;}

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

void	client::printAttr() const {
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
void client::parseBody(std::string body)
{
	http_request["Body"] = body;
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
	if (pos + 2 < request.size())
		parseBody(request.substr(pos + 2));
	if (checkMandatoryElements())
		return ;
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
struct compare
{
	std::string name;
	client c;
	compare(std::string my_, client c_): name(my_), c(c_) {}
 
	bool operator()(serverconfig s) {
		if (name != "")
			return (s.getServerName() == name && s.getListen() == c.getPort());
		else
			return s.getListen() == c.getPort();
	}
};

void client::matchHost(std::vector<serverconfig> hosts)
{
	std::string name = http_request["Host"];
	size_t f = name.find(":");
	if (f != std::string::npos)
		name = name.substr(0, f);
	std::vector<serverconfig>::iterator s = std::find_if(hosts.begin(), hosts.end(), compare(name, *this));
	if (s != hosts.end())
		host = *s;
	else
		host = *(std::find_if(hosts.begin(), hosts.end(), compare("", *this)));
}

serverconfig& client::getHost(void)
{
	return (this->host);
}

void client::setFirstTime(bool b) {first_time = b;}

bool& client::getFirstTime() {return first_time;}

int& client::getReadfds() {return readfds;}

void client::setReadfds(int i) {readfds = i;}