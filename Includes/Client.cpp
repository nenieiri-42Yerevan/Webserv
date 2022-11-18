/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 16:38:07 by vismaily          #+#    #+#             */
/*   Updated: 2022/11/17 19:31:03 by vismaily         ###   ########.fr       */
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
	this->_isLocation = false;
}

Client::Client(std::vector<Server> &serverSet, int serverNumber)
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
	this->_server = serverSet[serverNumber];
	this->_isLocation = false;
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
	this->_server= other._server;
	this->_isLocation = other._isLocation;
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
		this->_server= rhs._server;
		this->_isLocation = rhs._isLocation;
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
		return (getError(400));
	this->_header.insert(std::make_pair("method", line.substr(0, pos)));
	if (this->_header["method"] == "" || \
		this->_header["method"].find(" \t\v\f") != std::string::npos)
		return (getError(400));
	pos = line.find_first_not_of(" ", pos);
	if (pos == std::string::npos)
		return (getError(400));
	line = line.substr(pos, line.length() - pos);
	pos = line.find_first_of(" ");
	if (pos == std::string::npos)
		return (getError(400));
	this->_header.insert(std::make_pair("uri", line.substr(0, pos)));
	if (this->_header["uri"] == "" || \
		this->_header["uri"].find(" \t\v\f") != std::string::npos)
		return (getError(400));
	pos = line.find_first_not_of(" ", pos);
	if (pos == std::string::npos)
		return (getError(400));
	line = line.substr(pos, line.length() - pos);
	this->_header.insert(std::make_pair("protocol-version", line));
	if (this->_header["protocol-version"] == "" || \
		this->_header["protocol-version"].find(" \t\v\f") != std::string::npos)
		return (getError(400));
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
	std::string										full_path;

	host = this->_header.find("host");
	if (host == this->_header.end())
		getError(400);
	else
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
		if (this->findServer() != 0)
		{
			full_path = this->_header.find("uri")->second;
			pos = full_path.find_last_of("/.");
			if (full_path[pos] == '/' && full_path[full_path.length() - 1] != '/')
				full_path += "/";
			this->findLocation(full_path);
			/* stugel */
			this->findFile(full_path, pos);
			/* code */
		}
	}
}

int	Client::findServer()
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
				return (1);
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
				return (1);
			}
		}
	}
	return (getError(400));
}

void	Client::findLocation(std::string &full_path)
{
	std::map<t_str, Location>::const_iterator	it_begin;
	std::map<t_str, Location>::const_iterator	it_end;
	std::pair<t_str, Location>					tmp;
   
	if (this->_isLocation == true)
	{
		if (_location.second.getLocation().size() == 0)
			return ;
		it_begin = _location.second.getLocation().begin();
		it_end = _location.second.getLocation().end();
	}
	else
	{
		if (_server.getLocation().size() == 0)
			return ;
		it_begin = _server.getLocation().begin();
		it_end = _server.getLocation().end();
	}
	while (it_begin != it_end)
	{
		if (std::equal(it_begin->first.begin(), it_begin->first.end(), \
													full_path.begin()) == 1)
		{
			if (tmp.first.length() < it_begin->first.length())
				tmp = *it_begin;
		}
		++it_begin;
	}
	if (tmp.first.length() != 0)
	{
		this->_location = tmp;
		this->_isLocation = true;
		findLocation(full_path);
	}
}

bool	Client::findFile(std::string &full_path, std::string::size_type pos)
{
	std::vector<t_str>::const_iterator	it_begin;
	std::vector<t_str>::const_iterator	it_end;
	std::string							tmp;
	std::string							root;
	std::string::size_type				len;

	if (full_path[pos] == '/')
	{
		if (this->_isLocation == true)
		{
			len = this->_location.first.length();
			full_path = full_path.substr(len, full_path.length() - len);
			full_path = this->_location.second.getRoot() + full_path;
			it_begin = this->_location.second.getIndex().begin();
			it_end = this->_location.second.getIndex().end();
		}
		else
		{
			if (this->_server.getRoot()[this->_server.getRoot().length() - 1] == '/')
				root = this->_server.getRoot().substr(0, \
						this->_server.getRoot().length() - 1);
			else
				root = this->_server.getRoot();
			full_path = root + full_path;
			it_begin = this->_server.getIndex().begin();
			it_end = this->_server.getIndex().end();
		}
		for (; it_begin != it_end; ++it_begin)
		{
			/* stugel*/
			tmp = full_path + *it_begin;
			std::cout << tmp << std::endl;
		}
	}
	else
	{
		if (this->_isLocation == true)
		{
			len = this->_location.first.length();
			full_path = full_path.substr(len, full_path.length() - len);
			full_path = this->_location.second.getRoot() + full_path;
		}
		else
		{
			if (this->_server.getRoot()[this->_server.getRoot().length() - 1] == '/')
				root = this->_server.getRoot().substr(0, \
						this->_server.getRoot().length() - 1);
			else
				root = this->_server.getRoot();
			full_path = root + full_path;
		}
			/* stugel*/
			tmp = full_path;
			std::cout << tmp << std::endl;
	}
	return (true);
}

int	Client::getError(int num)
{
	switch (num)
	{
		case 400:
			getErrorMsg(400, "400", "Bad Request");
			break ;
		default:
			return (0);
	}
	return (0);
}

bool	Client::readWhole(const std::string &full_path, \
							std::string &readFile) const
{
	std::ifstream			openFile;
	std::ostringstream		ss;

	openFile.open(full_path.c_str());
	if (!openFile.is_open())
		return (false);
	ss << openFile.rdbuf();
	readFile = ss.str();
	openFile.close();
	return (true);
}

bool	Client::responseErrorPage(int errNum, std::string &response_body) const
{
	std::map<int, std::string>::const_iterator	it;
	std::string									full_path;

	if (this->_isLocation == true)
	{
		it = this->_location.second.getErrorPage().find(errNum);
		if (it == this->_location.second.getErrorPage().end())
			return (false);
		full_path = this->_location.second.getRoot() + it->second;
	}
	else
	{
		it = this->_server.getErrorPage().find(errNum);
		if (it == this->_server.getErrorPage().end())
			return (false);
		full_path = this->_server.getRoot() + it->second;
	}
	if (access(full_path.c_str(), F_OK | R_OK) != 0)
		return (false);
	return (readWhole(full_path, response_body));
}

void	Client::getErrorMsg(int errNum, const t_str &num, const t_str &msg)
{
	std::string	response;
	std::string	response_body;

	if (responseErrorPage(errNum, response_body) == false)
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
	this->_response = response;
}
