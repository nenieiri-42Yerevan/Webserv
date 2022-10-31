/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/30 15:37:59 by vismaily          #+#    #+#             */
/*   Updated: 2022/10/31 12:43:36 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

std::vector<Server>	*parsing(std::string &config)
{
	std::vector<Server>		*server_set;
	std::string::size_type	found_server;
	std::string				body;

	server_set = new std::vector<Server>;
	try
	{
		while (config != "")
		{
			found_server = config.find_first_not_of(" \t\v\r\n\f");
			if (found_server == std::string::npos)
				break ;
			else if (config.compare(found_server, 6, "server") == 0)
			{
				body = get_inside(config, found_server + 6);
				server_set->push_back(Server(body));
			}
			else
				throw std::runtime_error("Error: Config file must contain only "
										 "'server' directive.");
		}
	}
	catch (...)
	{
		delete server_set;
		throw ;
	}
	return (server_set);
}
