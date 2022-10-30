/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_server.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 15:26:22 by vismaily          #+#    #+#             */
/*   Updated: 2022/10/30 16:33:31 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	start_server(std::string &config)
{
	std::vector<Server>		*server_set;

	server_set = parsing(config);
	/* code */
	delete server_set;
}
