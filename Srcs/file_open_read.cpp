/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_open.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 13:47:46 by vismaily          #+#    #+#             */
/*   Updated: 2022/10/28 17:07:34 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# include "webserv.hpp"

int	file_open_read(std::string config_file, std::string &config)
{
	std::string::size_type	found;
	std::string				ext;
	std::ifstream			config_stream;
	std::ostringstream		ss;

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
		ss << config_stream.rdbuf();
		config= ss.str();
		config_stream.close();
	}
	return (0);
}
