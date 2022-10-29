/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_server.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 15:26:22 by vismaily          #+#    #+#             */
/*   Updated: 2022/10/29 18:40:22 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static std::string	get_server_inside(std::string &config, \
								std::string::size_type found_server)
{
	std::string::size_type	found_first_brec;
	std::string::size_type	found_last_brec;
	std::string::size_type	count;
	std::string				body;

	count = 1;
	found_first_brec = config.find_first_not_of(" \t\v\r\n\f", found_server);
	if (config[found_first_brec] != '{')
	{
		std::cerr << "Error: directive 'server' has no opening '{'";
		std::cerr << std::endl;
		body = "";
		return (body);
	}
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
	{
		std::cerr << "Error: directive 'server' has no closing '}'";
		std::cerr << std::endl;
		body = "";
		return (body);
	}
	body = config.substr(found_first_brec, found_last_brec - found_first_brec + 1);
	config.erase(0, found_last_brec + 1);
	return (body);
}

void	start_server(std::string &config)
{
	std::string::size_type	found_server;

//	while (config != "")
//	{
		found_server = config.find_first_not_of(" \t\v\r\n\f");
		if (config.compare(found_server, 6, "server") == 0)
		{
			if (get_server_inside(config, found_server + 6) == "")
				return ;
		}
		else
		{
			std::cerr << "Error: Config file must contain only 'server' ";
			std::cerr << "directive." << std::endl;
			return ;
		}
//	}
	std::cout << config << std::endl;
}
