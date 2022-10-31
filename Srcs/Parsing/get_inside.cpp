/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_inside.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/30 14:32:00 by vismaily          #+#    #+#             */
/*   Updated: 2022/10/31 14:09:20 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

std::string	get_inside(std::string &config, std::string::size_type start_pos)
{
	std::string::size_type	found_first_brec;
	std::string::size_type	found_last_brec;
	std::string::size_type	count;
	std::string				body;

	count = 1;
	found_first_brec = config.find_first_not_of(" \t\v\r\n\f", start_pos);
	if (config[found_first_brec] != '{')
		throw std::runtime_error("Error: Config file: directive 'server' " \
								 "has no opening '{'.");
	found_last_brec = config.find_first_of("{}", found_first_brec + 1);
	while (found_last_brec != std::string::npos && count != 0)
	{
		if (config[found_last_brec] == '{')
			++count;
		else
			--count;
		if (count != 0)
			found_last_brec = config.find_first_of("{}", found_last_brec + 1);
	}
	if (found_last_brec == std::string::npos)
		throw std::runtime_error("Error: Config file: directive 'server' " \
								 "has no closing '}'.");
	body = config.substr(found_first_brec, found_last_brec - found_first_brec + 1);
	config.erase(0, found_last_brec + 1);
	return (body);
}
