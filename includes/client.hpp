/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchliyah <mchliyah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 06:23:43 by slahrach          #+#    #+#             */
/*   Updated: 2023/03/28 08:22:37 by mchliyah         ###   ########.fr       */
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
public:
	client(int sock, std::string port_);
	int			getSocket() const;
	std::string getPort() const;
	bool		getIsSent() const;
	void		setIsSent(bool a);
	void setRequest(char *req);
	int parseRequestLine(std::string first_line);
	void parseHeader(std::string header);
	void parseBody(std::string request);
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
	~client();
};
#endif
