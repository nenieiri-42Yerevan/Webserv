/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_open.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 13:47:46 by vismaily          #+#    #+#             */
/*   Updated: 2022/10/28 14:55:26 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "webserv.hpp"

int	file_open(std::string config_file)
{
	std::string::size_type	found;
	std::string				ext;
	std::ifstream			config;

	ext = CONFIG_EXTANSION;
	found = config_file.rfind(CONFIG_EXTANSION);
	if (found == std::string::npos || config_file[found + ext.length()] != '\0')
	{
		std::cout << "Error: The config file must end with ";
		std::cout << CONFIG_EXTANSION << " extension." << std::endl;
		return (-1);
	}
	else
	{
		if (config_file.find('/') == std::string::npos)
			config_file.insert(0, CONFIG_PATH);
		config.open(config_file);
		if (!config.is_open())
		{
			std::cout << "Error: Failed to open the file." << std::endl;
			return (-1);
		}
	}
	return (0);
}
