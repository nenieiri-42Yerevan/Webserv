/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 16:42:16 by vismaily          #+#    #+#             */
/*   Updated: 2022/10/31 19:06:51 by vismaily         ###   ########.fr       */
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
	str::size_type		found_name_begin;
	str::size_type		found_name_end;
	str::size_type		found_value_begin;
	str::size_type		found_value_end;
	str::size_type		count = 1;
	std::string			tmp;

	while (body != "")
	{
		found_name_begin = body.find_first_not_of(" \t\v\r\n\f");
		if (found_name_begin == str::npos)
			break ;
		found_name_end = body.find_first_of(" \t\v\r\n\f", found_name_begin);
		if (found_name_end == str::npos)
			throw std::logic_error("Error: Config file: after directive name " \
								   "must be at least one whitespce and " \
								   "after that must be its value.");
		found_value_begin = body.find_first_not_of(" \t\v\r\n\f", found_name_end);
		if (found_value_begin == str::npos)
		{
			tmp = "Error: Config file: directive '";
			tmp += body.substr(found_name_begin, found_name_end - found_name_begin);
			tmp += "' doesn't have a value.";
			throw std::logic_error(tmp);
		}
		/* write if is valid name */
		if (body.compare(found_name_begin, 8, "location") != 0)
		{
			found_value_end = body.find_first_of(';', found_value_begin);
			if (found_value_end == str::npos)
			{
				tmp = "Error: Config file: value of directive '";
				tmp += body.substr(found_name_begin, found_name_end - found_name_begin);
				tmp += "' must end with ';' symbol.";
				throw std::logic_error(tmp);
			}
		}
		else
		{
			found_value_end = body.find_first_of("{}", found_value_begin + 1);
			if (found_value_end == str::npos || body[found_value_end] != '{')
				throw std::runtime_error("Error: Config file: directive " \
										 "'location' has no opening '{'.");
			found_value_end = body.find_first_of("{}", found_value_end + 1);
			while (found_value_end != str::npos && count != 0)
			{
				if (body[found_value_end] == '{')
					++count;
				else
					--count;
				if (count != 0)
					found_value_end = body.find_first_of("{}", found_value_end + 1);
			}
			if (found_value_end == str::npos)
				throw std::runtime_error("Error: Config file: directive " \
										 "'location' has no closing '}'.");
			++found_value_end;
		}
		/* send to init */
		body.erase(0, found_value_end + 1);
	}
}
