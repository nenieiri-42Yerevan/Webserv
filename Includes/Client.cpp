/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 16:38:07 by vismaily          #+#    #+#             */
/*   Updated: 2022/11/07 14:56:58 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"

/*=====================================*/
/*     Constructors and Destructor     */
/*=====================================*/

Client::Client()
{
	this->_request = "";
	this->_body = "";
	this->_isStart = 0;
	this->_isHeader = 0;
	this->_lastHeader = "";
}

Client::Client(const Client &other)
{
	this->_request = other._request;
	this->_header = other._header;
	this->_body = other._body;
	this->_isStart = other._isStart;
	this->_isHeader = other._isHeader;
	this->_lastHeader = other._lastHeader;
}

Client	&Client::operator=(const Client &rhs)
{
	if (this != &rhs)
	{
		this->_request = rhs._request;
		this->_header = rhs._header;
		this->_body = rhs._body;
		this->_isStart = other._isStart;
		this->_isHeader = other._isHeader;
		this->_lastHeader = other._lastHeader;
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
	begin = line.find_first_not_of(" \t\v\r\n\f");
	if (begin != 0)
	{
		end = line.find_last_not_of(" \t\v\r\n\f", line.length() - 1);
		if (end == std::string::npos)
			return ;
		end += 1;
		header_value = line.substr(begin, end - begin);
		if (this->_lastHeader != "")
		{
			this->_header[_lastHeader] += " ";
			this->_header[_lastHeader] += header_value;
		}
	}
	else
	{
		colon = line.find(":");
		if (colon != std::string::npos)
		{
			begin = 0;
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
			 */
			this->_header.insert(std::make_pair(header_name, header_value));
			this->_lastHeader = header_name;
		}
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
