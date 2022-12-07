/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 13:17:05 by vismaily          #+#    #+#             */
/*   Updated: 2022/12/07 14:48:39 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int	main(int argc, char **argv)
{
	std::string	config;

	try
	{
		if (argc == 1)
			file_open_read(CONFIG_DEFAULT, config);
		else if (argc == 2)
			file_open_read(argv[1], config);
		else
			throw std::runtime_error("Error: Argumets count must be 1 or 0.");
		start_server(config);
	}
	catch (const std::exception &e)
	{
		std::cerr << e.what() <<  std::endl;
	}
	return (0);
}
