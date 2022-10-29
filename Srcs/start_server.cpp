/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_server.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 15:26:22 by vismaily          #+#    #+#             */
/*   Updated: 2022/10/29 17:36:11 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	start_server(std::string &config)
{
	std::string::size_type	found;

//	while (config != "")
//	{
		found = config.find_first_not_of(" \t\v\r\n\f");
		if (config.compare(found, 6, "server") == 0)
		{
			found = config.find_first_not_of(" \t\v\r\n\f", found + 6);
			if (config[found] != '{')
			{
				std::cerr << "Error: directive 'server' has no opening '{'";
				std::cerr << std::endl;
				return ;
			}
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
