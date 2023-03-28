/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/28 06:23:43 by slahrach          #+#    #+#             */
/*   Updated: 2023/03/28 09:09:41 by slahrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef REQUEST_HPP
# define REQUEST_HPP
#include <map>
#include <string>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cstdio>
#include "config.hpp"

class client
{
private:
	std::string	request;
	std::string	port;
	int			socket_fd;
	bool		isSent;
	int			error;
	std::string	err_message;
	std::map<std::string, std::string> http_request;
	serverconfig host;
public:
	int	rcv;
	client(int sock, std::string port_);
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
	serverconfig getHost(void);
	void addToBody(std::string body);
	void addToRequestCheck(std::string buff);
	~client();
};
#endif
