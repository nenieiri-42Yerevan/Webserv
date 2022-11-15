/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 16:38:07 by vismaily          #+#    #+#             */
/*   Updated: 2022/11/15 14:25:15 by vismaily         ###   ########.fr       */
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
	this->_isRecvFinish = false;
	this->_isSendFinish = false;
	this->_version = "webserv/1.0.0";
	this->_isStart = 0;
	this->_isHeader = 0;
	this->_lastHeader = "";
}

Client::Client(std::vector<Server> &serverSet)
{
	this->_request = "";
	this->_body = "";
	this->_isRecvFinish = false;
	this->_isSendFinish = false;
	this->_version = "webserv/1.0.0";
	this->_isStart = 0;
	this->_isHeader = 0;
	this->_lastHeader = "";
	this->_serverSet = serverSet;
}

Client::Client(const Client &other)
{
	this->_request = other._request;
	this->_response = other._response;
	this->_header = other._header;
	this->_body = other._body;
	this->_isRecvFinish = other._isRecvFinish;
	this->_isSendFinish = other._isSendFinish;
	this->_version = other._version;
	this->_isStart = other._isStart;
	this->_isHeader = other._isHeader;
	this->_lastHeader = other._lastHeader;
	this->_serverSet = other._serverSet;
}

Client	&Client::operator=(const Client &rhs)
{
	if (this != &rhs)
	{
		this->_request = rhs._request;
		this->_response = rhs._response;
		this->_header = rhs._header;
		this->_body = rhs._body;
		this->_isRecvFinish = rhs._isRecvFinish;
		this->_isSendFinish = rhs._isSendFinish;
		this->_version = rhs._version;
		this->_isStart = rhs._isStart;
		this->_isHeader = rhs._isHeader;
		this->_lastHeader = rhs._lastHeader;
		this->_serverSet = rhs._serverSet;
	}
	return (*this);
}

Client::~Client()
{
}

/*=====================================*/
/*          Setter and Getters         */
/*=====================================*/

void	Client::setRequest(const std::string &request)
{
	this->_request += request;
	parsing();
}

bool	Client::getRecvStatus() const
{
	return (this->_isRecvFinish);
}

bool	Client::getSendStatus() const
{
	return (this->_isSendFinish);
}

const std::string	Client::getResponse()
{
	std::string	tmp;

	tmp = _response;
	_response = "";
	this->_isSendFinish = true;
	return (tmp);
}

const std::string	Client::getResponse(size_t buff_size)
{
	std::string	tmp;

	tmp = _response.substr(0, buff_size);
	if (buff_size > _response.length())
		_response = "";
	else
		_response = _response.substr(buff_size, _response.length() - buff_size);
	if (_response == "")
		this->_isSendFinish = true;
	return (tmp);
}

/*=====================================*/
/*       Other Member Functions        */
/*=====================================*/

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
				{
					if (parsingRequestLine(_request.substr(0, pos)) == 0)
					{
						this->_isRecvFinish = true;
						return ;
					}
				}
				else
					parsingHeader(_request.substr(0, pos));
				_request = _request.substr(pos, _request.length() - pos);
			}
			pos = _request.find("\r\n");
		}
		++_isHeader;
		parsingBody();
	}
	else
		parsingBody();
}

int	Client::parsingRequestLine(std::string line)
{
	std::string::size_type	pos;
	pos = line.find_first_of(" ");
	if (pos == std::string::npos)
	{
		_response = getError(400);
		return (0);
	}
	this->_header.insert(std::make_pair("method", line.substr(0, pos)));
	if (this->_header["method"] == "" || \
		this->_header["method"].find(" \t\v\f") != std::string::npos)
	{
		_response = getError(400);
		return (0);
	}
	pos = line.find_first_not_of(" ", pos);
	if (pos == std::string::npos)
	{
		_response = getError(400);
		return (0);
	}
	line = line.substr(pos, line.length() - pos);
	pos = line.find_first_of(" ");
	if (pos == std::string::npos)
	{
		_response = getError(400);
		return (0);
	}
	this->_header.insert(std::make_pair("uri", line.substr(0, pos)));
	if (this->_header["uri"] == "" || \
		this->_header["uri"].find(" \t\v\f") != std::string::npos)
	{
		_response = getError(400);
		return (0);
	}
	pos = line.find_first_not_of(" ", pos);
	if (pos == std::string::npos)
	{
		_response = getError(400);
		return (0);
	}
	line = line.substr(pos, line.length() - pos);
	this->_header.insert(std::make_pair("protocol-version", line));
	if (this->_header["protocol-version"] == "" || \
		this->_header["protocol-version"].find(" \t\v\f") != std::string::npos)
	{
		_response = getError(400);
		return (0);
	}
	++_isStart;
	return (1);
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
	this->_isRecvFinish = true;
	this->prepareAnswer();
}

void	Client::prepareAnswer()
{
	std::map<std::string, std::string>::iterator	host;
	std::string::size_type							pos;

	host = this->_header.find("host");
	(void) pos;
	//if (host == this->_header.end())
		this->_response = getError(400);
	/*else
	{
		pos = host->second.find(":");
		if (pos == std::string::npos)
			this->_port = "80";
		else
		{
			++pos;
			this->_port = host->second.substr(pos, host->second.length() - pos);
			host->second = host->second.substr(0, pos - 1);
		}
		this->_host = host->second;
		this->findServer();
	}*/
}

void	Client::findServer()
{
	std::multimap<std::string, std::string>				listen;
	std::multimap<std::string, std::string>::iterator	listen_it;
	std::vector<std::string>							servName;
	std::vector<std::string>::iterator					serv_it;

	for (std::vector<Server>::size_type i = 0; i < this->_serverSet.size(); ++i)
	{
		listen = this->_serverSet[i].getListen();
		for (listen_it = listen.begin(); listen_it != listen.end(); ++listen_it)
		{
			if (listen_it->first == this->_host && listen_it->second == this->_port)
			{
				this->_server = this->_serverSet[i];
				return ;
			}
		}
	}
	for (std::vector<Server>::size_type i = 0; i < this->_serverSet.size(); ++i)
	{
		servName = this->_serverSet[i].getServerName();
		for (serv_it = servName.begin(); serv_it != servName.end(); ++serv_it)
		{
			if (*serv_it == this->_host)
			{
				this->_server = this->_serverSet[i];
				return ;
			}
		}
	}
//	this->_serverNumber = -1;
}

std::string	Client::getError(int num)
{
	switch (num)
	{
		case 400:
			return (getErrorMsg("400", "Bad Request"));
		default:
			return ("");
	}
}

bool	Client::responseErrorPage(std::string &response_body)
{
	(void)response_body;
	return (false);
//	return (true);
}

std::string	Client::getErrorMsg(const t_str &num, const t_str &msg)
{
	std::string	response;
	std::string	response_body;

	if (responseErrorPage(response_body) == false)
	{
		response_body += "<html>";
		response_body += "<head><title>" + num + " " + msg + "</title></head>";
		response_body += "<body>";
		response_body += "<center><h1>" + num + " " + msg + "</h1></center><hr>";
		response_body += "<center>" + this->_version + "</center>";
		response_body += "</body>";
		response_body += "</html>";
	}

	std::stringstream	ss;
	ss << response_body.length();

	response += "HTTP/1.1 " + num + " " + msg + "\r\n";
	response += "Content-Type : text/html;\r\n";
	response += "Content-Length : " + ss.str() + "\r\n";
	response += "Server : webserv\r\n";
	response += "\r\n";
	response += response_body;
	return (response);
}
