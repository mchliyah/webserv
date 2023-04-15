/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 09:12:48 by slahrach          #+#    #+#             */
/*   Updated: 2023/04/14 02:14:50 by slahrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/client.hpp"

client::client(int sock, std::pair<std::string, std::string> ph) : request("") ,port_host(ph),socket_fd(sock), isSent(0), error(200), first_time(true), err_message(""), buff(""), bodyname(""), rcv(0), last_rcv(std::clock())
{
	snd = 0;
}

client::client(const client& other) : request(other.request), port_host(other.port_host), socket_fd(other.socket_fd), isSent(other.isSent), error(other.error), first_time(other.first_time), err_message(other.err_message), http_request(other.http_request), host(other.host), sent_bytes(other.sent_bytes), res(other.res), rcv(other.rcv), snd(other.snd), last_rcv(other.last_rcv)
{
}

void client::setBuff(std::string buff_)
{
	buff = buff_;
}

std::string& client::getBuff()
{
	return buff;
}

client& client::operator=(const client& other)
{
	if (this != &other)
	{
		this->request = other.request;
		this->port_host = other.port_host;
		this->socket_fd = other.socket_fd;
		this->isSent = other.isSent;
		this->error = other.error;
		this->first_time = other.first_time;
		this->err_message = other.err_message;
		this->http_request = other.http_request;
		this->host = other.host;
		this->sent_bytes = other.sent_bytes;
		this->res = other.res;
		this->rcv = other.rcv;
		this->snd = other.snd;
	}
	return (*this);
}

client::~client(){}
//getters & setters
std::string generateString(int length) {
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::string str;
    for (int i = 0; i < length; ++i) {
        str += alphanum[rand() % (sizeof(alphanum) - 1)];
    }
    return str;
}

int& client::getSocket() {return socket_fd;}
bool& client::getIsSent() {return isSent;}
void client::setIsSent(bool b) {isSent = b;}
void client::resetClient()
{
	this->request = "";
	this->error = 200;
	this->err_message = "";
	this->http_request.clear();
	this->isSent = 0;
	this->rcv = 0;
	this->query = "";
	this->first_time = true;
	this->snd = 0;
	this->sent_bytes = 0;
	this->bodyname = "";
	if (this->file.is_open())
		this->file.close();
	res.clearall();
	this->multipart.clear();
	this->last_rcv = std::clock();
}
void client::makeError(int err, const std::string& msg)
{
	error= err;
	err_message = msg;
	rcv = 4;
}
int client::fileSize(std::string filename)
{
    std::ifstream file;
    struct stat st;
    int size;
    file.open(filename.c_str());
    if (stat(filename.c_str(), &st) != -1)
    {
        size = st.st_size;
        file.close();
        return (size);
    }
    file.close();
    return (-1);
}
void client::checkBodySize(void)
{
    if (error == 200)
    {
        std::string max = getHost().getMaxClientBodySize();
		if (max == "")
			return ;
        std::stringstream size(max);
        int maxBodySize;
        size >> maxBodySize;
        int sz = 0;
        if (http_request["body"] == "present")
            sz = fileSize(getBodyname());
        else if (http_request["body"] == "multipart")
        {
            for (std::vector<std::string>::iterator it = multipart.begin(); it != multipart.end(); it++)
                sz += fileSize(*it);
        }
        if (sz <= 0 || sz > maxBodySize)
            error = 413;
    }
}
int	client::checkMethod_URL()
{
	bool	found = 0;
	std::string	methods[3] = {"GET", "POST", "DELETE"};
	std::string method = getValue("Method");
	for (int i = 0; i < 3; i++)
		if (methods[i] == method)
			found = 1;
	if (!found)
	{
		makeError(501, "Bad Request: Method not implemented");
		return (1);
	}
	if (getValue("URL").length() > 1000)
	{
		makeError(414, "Bad Request: URL too long");
		return (1);
	}
	size_t pos = getValue("URL").find("?");
	if (pos != std::string::npos)
	{
		query = getValue("URL").substr(pos + 1);
		http_request["URL"] = getValue("URL").substr(0, pos);
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
	if (http_request["version"] != "HTTP/1.1" && http_request["version"] != "HTTP/1.0")
	{
		makeError(505, "HTTP Version Not Supported");
		return (1);
	}
	if (checkMethod_URL())
		return (1);
	return 0;
}
void client::parseHeader(std::string& header)
{
	std::string	key = "";
	std::string	value = "";
	size_t		pos;

	pos = header.find(":");
	key = header.substr(0, pos);
	if (pos + 1 < header.size() && header[pos + 1] == ' ')
		pos++;
	value = header.substr(pos + 1);
	if (key == "Cookie" && http_request[key] != "")
		value = http_request[key] + "; " + value;
	else
		http_request[key] = value;
}
void client::handleMultipart(void)
{
	std::string type = http_request["Content-Type"];
	if (type == "")
		return;
	if (type.find("multipart/form-data") != std::string::npos)
	{
		size_t pos = type.find("boundary=");
		if (pos != std::string::npos)
		{
			pos += 9;
			size_t found = type.find(";", pos);
			std::string boundry = type.substr(pos, found -pos);
			http_request["body"] = "multipart";
			std::ifstream file(getBodyname().c_str());
			if (!file.is_open())
			{
				error = 500;
				return ;
			}
			std::ofstream output;
			int track = 0;
			char	buf[1000000];
			std::string rest;
			while (1)
			{
				file.read(buf, 1000000);
				std::string buff(buf, buf + file.gcount());
				buff = rest + buff;
				if (track == 0)
				{
					size_t found = buff.find("--" + boundry + "\r\n");
					size_t pos = buff.find("\r\n\r\n", found + 4 + boundry.length());
					if (found == std::string::npos || pos == std::string::npos)
					{
						error = 400;
						return ;
					}
					found = found + 4 + boundry.length();
					std::string headers = buff.substr(found, pos - found);
					buff = buff.substr(pos + 4);
					std::string newfile = "";
					std::string quote(1, '"');
					size_t f = headers.find("filename=" + quote);
					if (f != std::string::npos)
					{
						f += 10;
						size_t q = headers.find(quote, f);
						if (q != std::string::npos)
							newfile = headers.substr(f, q - f);
					}
					if (newfile == "")
						newfile = generateString(5) + ".txt";
					this->multipart.push_back(newfile);
					if (output.is_open())
						output.close();
					output.open(newfile.c_str(), std::ios::out | std::ios::trunc);
					output.close();
					output.open(newfile.c_str(), std::ios::app);
					if (!output.is_open())
					{
						error = 500;
						return ;
					}
					track = 1;
				}
				if (track == 1)
				{
					size_t b = buff.find("\r\n--" + boundry + "\r\n");
					if (b != std::string::npos)
					{
						track = 0;
						rest = buff.substr(0, b);
						buff = buff.substr(b + 2);
						output << rest;
					}
					else if (buff.find("\r\n--" + boundry + "--\r\n") != std::string::npos)
					{
						b = buff.find("\r\n--" + boundry + "--\r\n");
						buff = buff.substr(0, b);
						output << buff;
						output.close();
						file.close();
						remove(getBodyname().c_str());
						return ;
					}
					else if (buff.find("\r") != std::string::npos)
					{
						b = buff.find("\r");
						rest = buff.substr(0, b);
						buff = buff.substr(b);
						output << rest;
					}
					else
					{
						output << buff;
						buff = "";
						}
					}
					rest = buff;
			}
		}
	}
}
void client::generateBodyName(void)
{
	std::string depo = http_request["Content-Disposition"];
	if (depo != "")
	{
		size_t f = depo.find("filename=");
		size_t pos = depo.find(";");
		if (pos== std::string::npos)
			pos = depo.length();
		if (f != std::string::npos)
		{
			f += 9;
			if (f < depo.length() && depo[f] == '"')
				f++;
			bodyname = depo.substr(f, pos - f);
			if (bodyname[bodyname.length() - 1] == '"')
				bodyname.erase(bodyname.end() - 1);
			return ;
		}
	}
	bodyname = generateString(5);
	std::string type = http_request["Content-Type"];
	if (type != "")
	{
		size_t p = type.find(";");
		if (p != std::string::npos)
			type.erase(p, type.length() - p);
		if (type == "text/plain")
		{
			bodyname += ".txt";
			return ;
		}
		size_t slash = type.find("/");
		if (slash != std::string::npos)
			bodyname = bodyname + "." + type.substr(slash + 1);
	}
}
void client::addToBody(std::string body)
{
	std::ofstream file;
	static bool chunking_track = 0;
	static size_t length = 0;
	static std::string chunked = "";
	if (rcv == 2)//first time
	{
		generateBodyName();
		file.open(bodyname.c_str(), std::ios::out | std::ios::trunc);
		if (file.is_open())
			file.close();
	}
	file.open(bodyname.c_str(), std::ios::app);
	if (!file.is_open())
	{
		error = 500;
		rcv = 4;
		return ;
	}
	if (http_request["Transfer-Encoding"] == "chunked")
	{
		chunked += body;
		while (chunked != "")
		{
			if (chunking_track == 0)
			{
				int start = 0;
				size_t found = chunked.find("\r\n");
				if (found == 0)
				{
					found = chunked.find("\r\n");
					start = 2;
				}
				if (found == std::string::npos)
				{
					rcv = 3;
					return;
				}
				std::string length_str = chunked.substr(start, found);
				length = strtol(length_str.c_str(), NULL, 16);
				if (length == 0)
				{
					chunking_track = 0;
					length = 0;
					chunked = "";
					rcv = 4;
					file.close();
					return;
				}
				chunked.erase(start, found + 2);
				chunking_track = 1;
			}
			if (chunking_track == 1)
			{
				size_t l = length;
				size_t i = 0;
				for (; i < chunked.length() && i < l; i++)
				{
					file << chunked[i];
					length--;
				}
				if (length == 0)
				{
					chunking_track = 0;
					i += 2;
				}
				chunked.erase(0, i);
			}
		}
		rcv = 3;
	}
	else if (http_request["Content-Length"] != "")
	{
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

int& client::getError() {return error;}

std::string& client::getErrorMessage() {return err_message;}

int client::checkMandatoryElements()
{
	if (getValue("Host").empty())
	{
		makeError(400, "Bad Request: Missing Host!");
		return (1);
	}
	return (0);
}

void client::matchHost(std::vector<serverconfig>& hosts)
{
	std::string name = http_request["Host"];
	size_t f = name.find(":");
	if (f != std::string::npos)
		name = name.substr(0, f);
	std::vector<serverconfig>::iterator s;
	for (s = hosts.begin(); s != hosts.end(); s++)
	{
		if (s->getServerName() == name && s->getListen() == port_host.first && s->getHostName() == port_host.second)
		{
			host = *s;
			break ;
		}
	}
	if (s == hosts.end())
	{
		for (s = hosts.begin(); s != hosts.end(); s++)
		{
			if (s->getListen() == port_host.first && s->getHostName() == port_host.second)
			{
				host = *s;
				break ;
			}
		}
	}
}

serverconfig& client::getHost(void) { return (this->host); }

void client::setFirstTime(bool b) {first_time = b;}

bool& client::getFirstTime() {return first_time;}

size_t& client::getSentBytes() {return sent_bytes;}

void client::setSentBytes(size_t sent_bytes) {this->sent_bytes = sent_bytes;}

void client::addToRequestCheck(std::string& buff)
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
std::string client::getQuery()const {return query;}

void client::setRes(const response &response) { this->res = response; }

response& client::getRes(void) { return (res); }
std::string& client::getRequest(void) { return (request); }
std::map<std::string, std::string>& client::getHttpRequest(void) { return (http_request); }
int& client::getRcv(void) { return (rcv); }
std::string& client::getBodyname() {return bodyname;}
void client::setError(int code){this->error = code;}
std::vector<std::string>& client::getMultipart() {return multipart;}
void client::removeFiles()
{
	for (std::vector<std::string>::iterator it = multipart.begin(); it != multipart.end(); it++)
	{
		std::string file = *it;
		remove(file.c_str());
	}
	remove(bodyname.c_str());
}