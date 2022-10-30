/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_open_read.cpp                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 13:47:46 by vismaily          #+#    #+#             */
/*   Updated: 2022/10/30 15:36:15 by vismaily         ###   ########.fr       */
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

void	file_open_read(std::string config_file, std::string &config)
{
	std::string::size_type	found;
	std::string				ext;
	std::ifstream			config_stream;
	std::string				error;

	ext = CONFIG_EXTANSION;
	found = config_file.rfind(ext);
	if (found == std::string::npos || config_file[found + ext.length()] != '\0')
	{
		error = "Error: The config file must end with '" + ext + "' extension.";
		throw std::runtime_error(error);
	}
	else
	{
		if (config_file.find('/') == std::string::npos)
			config_file.insert(0, CONFIG_PATH);
		config_stream.open(config_file.c_str());
		if (!config_stream.is_open())
			throw std::runtime_error("Error: Failed to open the file.");
		config_read_trim(config_stream, config);
		if (config == "")
			throw std::runtime_error("Error: Empty config file.");
		config_stream.close();
	}
}
