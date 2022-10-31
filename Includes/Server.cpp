/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 16:42:16 by vismaily          #+#    #+#             */
/*   Updated: 2022/10/31 16:58:08 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

/*=====================================*/
/*     Constructors and Destructor     */
/*=====================================*/

Server::Server()
{
	_serverName.push_back("");
}

Server::Server(std::string &body)
{
	std::string	tmp;

	tmp = body.substr(1, body.length() - 2);
	if (tmp == "" || tmp.find_first_not_of(" \t\v\r\n\f") == std::string::npos)
		throw std::logic_error("Error: Config file: directive 'server' is empty.");
	parsingBody(tmp);
}

Server::Server(const Server &other)
{
	this->_serverName = other._serverName;
}

Server	&Server::operator=(const Server &rhs)
{
	if (this != &rhs)
	{
		this->_serverName = rhs._serverName;
	}
	return (*this);
}

Server::~Server()
{
}

/*=====================================*/
/*          Setter and Getters         */
/*=====================================*/

const std::vector<std::string>	Server::getServerName() const
{
	return (this->_serverName);
}

/*=====================================*/
/*       Other Member Functions        */
/*=====================================*/

void	Server::parsingBody(std::string &body)
{
	typedef std::string	str;
	str::size_type		found_begin_name;
	str::size_type		found_end_name;
	str::size_type		found_begin_value;
	str::size_type		found_end_value;
	std::string			tmp;

	while (body != "")
	{
		found_begin_name = body.find_first_not_of(" \t\v\r\n\f");
		if (found_begin_name == str::npos)
			break ;
		found_end_name = body.find_first_of(" \t\v\r\n\f", found_begin_name);
		if (found_end_name == str::npos)
			throw std::logic_error("Error: Config file: after directive name " \
								   "must be at least one whitespce and " \
								   "after that must be its value.");
		found_begin_value = body.find_first_not_of(" \t\v\r\n\f", found_end_name);
		if (found_begin_value == str::npos)
		{
			tmp = "Error: Config file: directive '";
			tmp += body.substr(found_begin_name, found_end_name - found_begin_name);
			tmp += "' doesn't have a value.";
			throw std::logic_error(tmp);
		}
		if (body.compare(found_begin_name, 8, "location") != 0)
		{
			found_end_value = body.find_first_of(';', found_begin_value);
			if (found_end_value == str::npos)
			{
				tmp = "Error: Config file: value of directive '";
				tmp += body.substr(found_begin_name, found_end_name - found_begin_name);
				tmp += "' must end with ';' symbol.";
				throw std::logic_error(tmp);
			}
		}
		else
		{
		}
		body.erase(0, found_end_value + 1);
	}
}
