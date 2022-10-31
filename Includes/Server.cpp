/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 16:42:16 by vismaily          #+#    #+#             */
/*   Updated: 2022/10/31 14:07:32 by vismaily         ###   ########.fr       */
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
	str::size_type		found_begin;
	str::size_type		found_end;

	if (body == "" || body.find_first_not_of(" \t\v\r\n\f") == str::npos)
		throw std::logic_error("Error: Config file: directive 'server' is empty.");
	while (body != "")
	{
		found_begin = body.find_first_not_of(" \t\v\r\n\f");
		if (found_begin == str::npos)
			break ;
		found_end = body.find_first_of(" \t\v\r\n\f", found_begin);
		if (found_end == str::npos)
			throw std::logic_error("Error: Config file: after directive name " \
								   "must be at least one whitespce and " \
								   "after that must be its value.");
		body.erase(0, found_end);
	}
}
