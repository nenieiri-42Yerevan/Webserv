/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 13:17:05 by vismaily          #+#    #+#             */
/*   Updated: 2022/10/28 17:07:28 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

int	main(int argc, char **argv)
{
	int			is_open;
	std::string	config;

	if (argc == 1)
		is_open = file_open_read(CONFIG_DEFAULT, config);
	else if (argc == 2)
		is_open = file_open(argv[1], config);
	else
	{
		std::cout << "Error: Argumets count must be 1 or 0." << std::endl;
		return (0);
	}
	if (is_open != -1)
		std::cout << config << std::endl;
	//	start_server();
	return (0);
}
