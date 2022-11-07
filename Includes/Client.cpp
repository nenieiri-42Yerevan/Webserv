/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 16:38:07 by vismaily          #+#    #+#             */
/*   Updated: 2022/11/07 13:51:27 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/*=====================================*/
/*     Constructors and Destructor     */
/*=====================================*/

Client::Client()
{
	this->_isStart = 0;
}

Client::Client(const Client &other)
{
	this->_request = other._request;
}

Client	&Client::operator=(const Client &rhs)
{
	if (this != &rhs)
	{
		this->_request = rhs._request;
	}
	return (*this);
}

Client::~Client()
{
}

/*=====================================*/
/*          Setter and Getters         */
/*=====================================*/

void	Client::setStr(const std::string &request)
{
	this->_request += request;
	parsing();
}

/*=====================================*/
/*       Other Member Functions        */
/*=====================================*/

void	Client::parsingRequestLine(std::string line)
{
	++_isStart;
	std::cout << "AAA: " << line << std::endl;
}

void	Client::parsingHeader(std::string line)
{
	std::string::size_type	colon;
	std::string::size_type	begin;
	std::string::size_type	end;
	std::string				header_name;
	std::string				header_value;

	/* if start with space */
	colon = line.find(":");
	if (colon != std::string::npos)
	{
		begin = line.find_first_not_of(" \t\v\r\n\f");
		end = line.find_last_not_of(" \t\v\r\n\f", colon - 1);
		if (begin == std::string::npos || end == std::string::npos)
			return ;
		end += 1;
		header_name = line.substr(begin, end - begin);
		line = line.substr(colon + 1, line.length() - (colon + 1));
		begin = line.find_first_not_of(" \t\v\r\n\f");
		end = line.find_last_not_of(" \t\v\r\n\f", line.length() - 1);
		if (begin == std::string::npos || end == std::string::npos)
			return ;
		end += 1;
		header_value = line.substr(begin, end - begin);
		/*
		 * name to lowercase
		 * duplicate values
		 */
		this->_header.insert(std::make_pair(header_name, header_value));
	}
}

void	Client::parsingBody()
{
	this->_body += this->_request;
	this->_request = "";
}

void	Client::parsing()
{
	if (_isStart == 0)
	{
		std::string::size_type	pos = _request.find_first_not_of("\r\n");
		if (pos == std::string::npos)
			return ;
		else if (pos != 0)
			this->_request = _request.substr(pos, _request.length() - pos);
		_isStart = 1;
	}
	if (_isHeader == 0)
	{
		std::string::size_type	pos = _request.find("\r\n");
		while (pos != std::string::npos)
		{
			pos += 2;
			if (pos == 2)
			{
				_isHeader = 1;
				_request = _request.substr(pos, _request.length() - pos);
				break ;
			}
			else
			{
				if (_isStart == 1)
					parsingRequestLine(_request.substr(0, pos));
				else
					parsingHeader(_request.substr(0, pos));
				_request = _request.substr(pos, _request.length() - pos);
			}
			pos = _request.find("\r\n");
		}
	}
	else
		parsingBody();
}
