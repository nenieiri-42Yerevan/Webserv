/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_server.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 15:26:22 by vismaily          #+#    #+#             */
/*   Updated: 2022/11/13 14:33:30 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	start_server(std::string &config)
{
	std::vector<Server>		*server_set;

	server_set = parsing(config);
	HttpServer	serv(server_set);
	serv.run();
	delete server_set;
}
