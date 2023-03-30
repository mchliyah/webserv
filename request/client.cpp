/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchliyah <mchliyah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 09:12:48 by slahrach          #+#    #+#             */
/*   Updated: 2023/03/30 09:09:25 by mchliyah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/client.hpp"

client::client(int sock, std::string port_) : request("") ,port(port_),socket_fd(sock), isSent(0), error(0), first_time(true), err_message(""), rcv(0)
{
	snd = 0;
}
client::~client(){}
//getters & setters
int client::getSocket() const {return socket_fd;}
std::string client::getPort() const {return port;}
bool client::getIsSent() const {return isSent;}
void client::setIsSent(bool b) {isSent = b;}
void client::resetClient()
{
	this->request = "";
	this->error = 200;
	this->err_message = "";
	this->http_request.clear();
	this->isSent = 0;
	this->rcv = 0;
}
void client::makeError(int err, const std::string& msg)
{
	error= err;
	err_message = msg;
	rcv = 4;
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
	std::stringstream stream;
	static bool chunking_track = 0;
	static size_t length = 0;
	static std::string chunked = "";
	std::ofstream file;
	stream << socket_fd;
	std::string filename = "body" + stream.str() + ".txt";
	if (rcv == 2)//first time
	{
		file.open(filename, std::ios::out | std::ios::trunc);
		file.close();
	}
	if (http_request["Transfer-Encoding"] == "chunked")
	{
		chunked += body;
		if (chunked == "")
			return;
		size_t found = chunked.find("\r\n");
		if (found == std::string::npos && chunking_track == 0)
		{
			rcv = 3;
			return;
		}
		if (chunking_track == 0)
		{
			chunking_track = 1;
			std::string length_str = chunked.substr(0, found);
			length = strtol(length_str.c_str(), NULL, 16);
			if (length == 0)
			{
				chunking_track = 0;
				length = 0;
				chunked = "";
				rcv = 4;
				return;
			}
			chunked.erase(0, found + 2);
		}
		else
		{
			file.open(filename, std::ios::app);
			size_t l = length;
			size_t i = 0;
			for (;i < found  && i < chunked.length() && i < l; i++)
			{
				file << chunked[i];
				length--;
			}
			if (found != std::string::npos)
				i += 2;
			chunked.erase(0, i + 1);
			if (length == 0)
				chunking_track = 0;
		}
		rcv = 3;
	}
	else if (http_request["Content-Length"] != "")
	{
		file.open("body" + stream.str() + ".txt", std::ios::app);
		file.seekp(0, std::ios::end);
		std::streampos size = file.tellp();
		std::string length = http_request["Content-Length"];
		std::stringstream l(length);
		int length_int;
		l >> length_int;
		int s = length_int - size;
		for (int i = 0; i < (int)body.length() && i < s; i++)
			file << body[i];
		file.seekp(0, std::ios::end);
		int si = file.tellp();
		if (rcv != 4)
			rcv = 3;
		if (si == length_int)
			rcv = 4;
		file.close();
	}
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
	checkMandatoryElements();
}
std::string&	client::getValue(const std::string& key) { return (http_request[key]); }

int client::getError() const {return error;}

std::string client::getErrorMessage() const {return err_message;}

int client::checkMandatoryElements()
{
	if (getValue("Host").empty())
	{
		makeError(400, "Bad Request: Missing Host!");
		return (1);
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

serverconfig& client::getHost(void) { return (this->host); }

void client::setFirstTime(bool b) {first_time = b;}

bool& client::getFirstTime() {return first_time;}

size_t client::getSentBytes() const {return sent_bytes;}

void client::setSentBytes(size_t sent_bytes) {this->sent_bytes = sent_bytes;}


bool client::openFile(response &res, std::string& path)
{
	file.open(path.c_str());
	struct stat buf;
	if (file.is_open() && stat(path.c_str(), &buf) == 0)
	{
		res.set_content_length("Content-Length: " + std::to_string(buf.st_size) + "\r\n");
		res.set_content_type("Content-Type: "+ get_type(path) + " \r\n");
		res.set_header("HTTP/1.1 " + res.get_status_code() + " " + res.get_status_message() + "\r\n"
			+ res.get_date() +  res.get_content_type() + res.get_content_length());
		std::vector<std::string>::iterator it;
		for (it = res.get_headers().begin(); it != res.get_headers().end(); it++)
			res.add_to_header(*it);
		res.add_to_header("\r\n");
		sent_bytes = res.get_header().size();
	}
	else
	{
		res.set_body("<!DOCTYPE html><html><head>403 Forbidden</head><body><p>no permission</p></body></html>");
		res.set_content_length("Content-Length: " + std::to_string(res.get_body().size()) + "\r\n");
		res.set_status_code("404");
		res.set_status_message("Not Found");
		res.set_content_type("Content-Type: text/html \r\n");
		res.set_header("HTTP/1.1 " + res.get_status_code() + " " + res.get_status_message() + "\r\n"
			+ res.get_date() + res.get_content_type() + res.get_content_length());
		std::vector<std::string>::iterator it;
		for (it = res.get_headers().begin(); it != res.get_headers().end(); it++)
			res.add_to_header(*it);
		res.add_to_header("\r\n");
		sent_bytes = res.get_header().size() + res.get_body().size();
		isSent = 1;
		return (false);
	}
	return (true);
}

bool client::readFile(response &res)
{
	char buff[BUF_SIZE + 1] = {0};
	if (!file.eof())
	{
		res.get_body().clear();
		file.read(buff, BUF_SIZE);
		std::cout << "read " << file.gcount() << " bytes" << std::endl;
		if (file.gcount() == 0)
			return (false);
		res.set_body(res.get_body().append(buff, 0, file.gcount()));
		sent_bytes = file.gcount();
	}
	else 
	{
		setFirstTime(true);
		setIsSent(1);
		file.close();
		res.get_body().clear();
		return (false);
	}
	return (true);
}

client::client(const client &other) { *this = other; }

client& client::operator=(const client &other)
{
	if (this != &other)
	{
		this->request = other.request;
		this->port = other.port;
		this->socket_fd = other.socket_fd;
		this->isSent = other.isSent;
		this->error = other.error;
		this->first_time = other.first_time;
		this->err_message = other.err_message;
		this->http_request = other.http_request;
		this->host = other.host;
		this->sent_bytes = other.sent_bytes;
	}
	return (*this);
}
void client::addToRequestCheck(std::string buff)
{
	std::string rest = "";
	request += buff;
	if (request.find("\r\n\r\n") != std::string::npos)
	{
		rcv = 1;
		rest = request.substr(request.find("\r\n\r\n") + 4);
		request = request.substr(0, request.find("\r\n\r\n") + 4);
		parse();
		if (http_request["Content-Length"] != "" || http_request["Transfer-Encoding"] == "chunked")
		{
			if (http_request["Content-Length"] == "0")
			{
				rcv = 4;
				return;
			}
			http_request["body"] = "present";
			if (rest != "")
			{
				rcv = 2;
				addToBody(rest);
			}
		}
		else
			rcv = 4;
	}
}
