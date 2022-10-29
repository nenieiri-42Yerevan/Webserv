/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 13:17:05 by vismaily          #+#    #+#             */
/*   Updated: 2022/10/29 15:00:37 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int	main(int argc, char **argv)
{
	int			res;
	std::string	config;

	if (argc == 1)
		res = file_open_read(CONFIG_DEFAULT, config);
	else if (argc == 2)
		res = file_open_read(argv[1], config);
	else
	{
		std::cout << "Error: Argumets count must be 1 or 0." << std::endl;
		return (0);
	}
//	if (res != -1)
//		start_server(config);
	return (0);
}
