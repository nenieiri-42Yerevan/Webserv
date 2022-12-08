/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_server.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 15:26:22 by vismaily          #+#    #+#             */
/*   Updated: 2022/12/08 19:54:24 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	start_server(std::string &config)
{
	std::vector<Server>		*server_set;

	server_set = parsing(config);
	try
	{
		HttpServer	serv(server_set);
		serv.run();
	}
	catch (...)
	{
	}
	delete server_set;
}
