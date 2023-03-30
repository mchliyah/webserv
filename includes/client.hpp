/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 06:23:43 by slahrach          #+#    #+#             */
/*   Updated: 2023/03/30 07:17:23 by slahrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP
# define BUF_SIZE 1024
#include <map>
#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cstdio>
#include "config.hpp"
#include "../includes/response.hpp"

class response;

class client
{
private:
	std::string	request;
	std::string	port;
	int			socket_fd;
	bool		isSent;
	int			error;
	bool		first_time;
	std::string	err_message;
	std::map<std::string, std::string> http_request;
	serverconfig host;
	std::ifstream file;
	size_t sent_bytes;
public:
	int	rcv;
	std::string query;
	client(int sock, std::string port_);
	client(const client& other);
	client& operator=(const client& );
	int			getSocket() const;
	std::string getPort() const;
	bool		getIsSent() const;
	void		setIsSent(bool a);
	void resetClient();
	int parseRequestLine(std::string first_line);
	void parseHeader(std::string header);
	void printAttr() const;
	void makeError(int err, const std::string& msg);
	std::string& getValue(const std::string& key);
	void parse();
	int checkMethod();
	int getError() const;
	std::string getErrorMessage() const;
	int checkMandatoryElements();
	void matchHost(std::vector<serverconfig> hosts);
	serverconfig& getHost(void);
	void setFirstTime(bool b);
	bool& getFirstTime(void);
	bool openFile(response &res, std::string &path);
	bool readFile(response &res);
	size_t getSentBytes() const;
	void setSentBytes(size_t sentBytes);
	void addToBody(std::string body);
	void addToRequestCheck(std::string buff);
	std::string getQuery() const;
	~client();
};
#endif
