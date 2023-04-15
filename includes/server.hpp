/* ************************************************************************** */
/*	                                                                    */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: slahrach <slahrach@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 08:44:49 by slahrach          #+#    #+#             */
/*   Updated: 2023/02/24 09:22:18 by slahrach         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

#include "config.hpp"
#include "client.hpp"
#include "response.hpp"

class server
{
	private:
		std::vector<std::pair<std::string, std::string> > ports_hosts;
		std::vector<std::pair<int, std::pair<std::string, std::string> > > listners;
		std::vector<serverconfig>& hosts;
		std::vector<client> clients;
	public:
		server(std::vector<std::pair<std::string, std::string> >ph, std::vector<serverconfig>& servers);
		void start();
		std::pair<int,std::pair<std::string, std::string> > createBindListen(std::pair<std::string, std::string>&ph);
		~server();
};

#endif
