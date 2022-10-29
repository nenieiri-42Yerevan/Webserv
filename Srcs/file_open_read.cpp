/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_open_read.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 13:47:46 by vismaily          #+#    #+#             */
/*   Updated: 2022/10/29 14:58:29 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

static void	config_read_trim(std::ifstream &config_stream, std::string &config)
{
	std::string::size_type	found;
	std::ostringstream		ss;

	ss << config_stream.rdbuf();
	config = ss.str();
	found = config.find_last_not_of(" \t\v\r\n\f");
	if (found != std::string::npos)
		config = config.substr(0, found + 1);
	found = config.find_first_not_of(" \t\v\r\n\f");
	if (found == std::string::npos)
		config = "";
	else if (found != 0)
		config = config.substr(found);
}

int	file_open_read(std::string config_file, std::string &config)
{
	std::string::size_type	found;
	std::string				ext;
	std::ifstream			config_stream;

	ext = CONFIG_EXTANSION;
	found = config_file.rfind(CONFIG_EXTANSION);
	if (found == std::string::npos || config_file[found + ext.length()] != '\0')
	{
		std::cout << "Error: The config file must end with '";
		std::cout << CONFIG_EXTANSION << "' extension." << std::endl;
		return (-1);
	}
	else
	{
		if (config_file.find('/') == std::string::npos)
			config_file.insert(0, CONFIG_PATH);
		config_stream.open(config_file);
		if (!config_stream.is_open())
		{
			std::cout << "Error: Failed to open the file." << std::endl;
			return (-1);
		}
		config_read_trim(config_stream, config);
		config_stream.close();
	}
	return (0);
}
