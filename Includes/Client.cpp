/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 16:38:07 by vismaily          #+#    #+#             */
/*   Updated: 2022/11/08 13:10:26 by vismaily         ###   ########.fr       */
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
	this->_isFinish = false;
	this->_isStart = 0;
	this->_isHeader = 0;
	this->_lastHeader = "";
}

Client::Client(const Client &other)
{
	this->_request = other._request;
	this->_header = other._header;
	this->_body = other._body;
	this->_isFinish = other._isFinish;
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
		this->_isFinish = rhs._isFinish;
		this->_isStart = rhs._isStart;
		this->_isHeader = rhs._isHeader;
		this->_lastHeader = rhs._lastHeader;
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

bool	Client::getStatus() const
{
	return (this->_isFinish);
}

/*=====================================*/
/*       Other Member Functions        */
/*=====================================*/

void	Client::parsingRequestLine(std::string line)
{
	std::string::size_type	pos;
	pos = line.find_first_of(" ");
	if (pos == std::string::npos)
		return ;
	this->_header.insert(std::make_pair("method", line.substr(0, pos)));
	if (this->_header["method"] == "" || \
		this->_header["method"].find(" \t\v\f") != std::string::npos)
		return ;
	pos = line.find_first_not_of(" ", pos);
	if (pos == std::string::npos)
		return ;
	line = line.substr(pos, line.length() - pos);
	pos = line.find_first_of(" ");
	if (pos == std::string::npos)
		return ;
	this->_header.insert(std::make_pair("uri", line.substr(0, pos)));
	if (this->_header["uri"] == "" || \
		this->_header["uri"].find(" \t\v\f") != std::string::npos)
		return ;
	pos = line.find_first_not_of(" ", pos);
	if (pos == std::string::npos)
		return ;
	line = line.substr(pos, line.length() - pos);
	this->_header.insert(std::make_pair("protocol-version", line));
	if (this->_header["protocol-version"] == "" || \
		this->_header["protocol-version"].find(" \t\v\f") != std::string::npos)
		return ;
	++_isStart;
}

void	Client::parsingHeader(std::string line)
{
	std::string::size_type	colon;
	std::string::size_type	begin;
	std::string::size_type	end;
	std::string				header_name;
	std::string				header_value;

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
			for (size_t	i = 0; i < header_name.length(); ++i)
				header_name[i] = tolower(header_name[i]);
			if (this->_header.find(header_name) == this->_header.end())
				this->_header.insert(std::make_pair(header_name, header_value));
			else if (this->_header[header_name].find(",") != std::string::npos)
				this->_header[header_name] += header_value;
			this->_lastHeader = header_name;
		}
	}
}

void	Client::parsingBody()
{
	this->_body += this->_request;
	this->_request = "";
	this->_isFinish = true;
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
