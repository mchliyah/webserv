/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.cpp                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mchliyah <mchliyah@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/11 23:15:41 by mchliyah          #+#    #+#             */
/*   Updated: 2023/03/11 23:16:46 by mchliyah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

bool has_only_spaces(std::string str)
{
	std::string::iterator it = str.begin();
	while (it != str.end())
	{
		if (*it != ' ' && *it != '\t')
			return false;
		it++;
	}
	return true;
}

int tab_count(std::string str) {
	int count = 0;
	while (str[count] == '\t')
		count++;
	return (count);
}