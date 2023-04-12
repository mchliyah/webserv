/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchliyah <mchliyah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 06:23:43 by slahrach          #+#    #+#             */
/*   Updated: 2023/04/12 05:44:46 by mchliyah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
#define REQUEST_HPP
#define BUF_SIZE 1024
#include <map>
#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <ctime>
#include "config.hpp"
#include "../includes/response.hpp"

class response;

class client
{
private:
	std::string request;
	std::string port;
	int socket_fd;
	bool isSent;
	int error;
	bool first_time;
	std::string err_message;
	std::map<std::string, std::string> http_request;
	std::string query;
	serverconfig host;
	std::ifstream file;
	size_t sent_bytes;
	response res;
	std::string buff;
	std::string bodyname;
	std::vector<std::string>multipart;

public:
	int rcv;
	int snd;
	std::clock_t last_rcv;
	client(int sock, std::string &port_);
	client(const client &other);
	client &operator=(const client &other);
	int &getSocket();
	void setError(int code);
	std::string &getPort();
	bool &getIsSent();
	void setIsSent(bool a);
	void resetClient();
	int parseRequestLine(std::string first_line);
	void parseHeader(std::string &header);
	void printAttr() const;
	void makeError(int err, const std::string &msg);
	std::string &getValue(const std::string &key);
	void parse();
	int checkMethod_URL();
	int &getError();
	std::string &getErrorMessage();
	int checkMandatoryElements();
	void matchHost(std::vector<serverconfig> &hosts);
	serverconfig &getHost(void);
	void setFirstTime(bool b);
	bool &getFirstTime(void);
	bool openFile(response &res, std::string &path);
	bool readFile(response &res);
	size_t &getSentBytes();
	void setSentBytes(size_t sentBytes);
	std::string getQuery() const;
	void addToBody(std::string body);
	void addToRequestCheck(std::string &buff);
	void setRes(const response &response);
	int &getRcv();
	std::string &getRequest();
	response &getRes(void);
	std::map<std::string, std::string> &getHttpRequest();
	void setBuff(std::string buff);
	void generateBodyName(void);
	int fileSize(std::string filename);
	void checkBodySize(void);
	void handleMultipart(void);
	std::string& getBuff();
	std::string& getBodyname();
	std::vector<std::string>& getMultipart();
	void errorResponse(response &res);
	void defaultResponse(response &res);
	void cgi_response(response &res, std::string& file_path, bool php);
	void removeFiles();
	~client();
};
std::string get_type(const std::string &path);
#endif
