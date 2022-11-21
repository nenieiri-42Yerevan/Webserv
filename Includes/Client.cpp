/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 16:38:07 by vismaily          #+#    #+#             */
/*   Updated: 2022/11/21 15:01:16 by vismaily         ###   ########.fr       */
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
	this->_isCGI = false;
	this->_contentLength = 0;
	this->_supportedMethods.push_back("GET");
	this->_supportedMethods.push_back("POST");
	this->_supportedMethods.push_back("DELETE");
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
	this->_isCGI = false;
	this->_contentLength = 0;
	this->_supportedMethods.push_back("GET");
	this->_supportedMethods.push_back("POST");
	this->_supportedMethods.push_back("DELETE");
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
	this->_supportedMethods = other._supportedMethods;
	this->_isCGI = other._isCGI;
	this->_contentLength = other._contentLength;
	this->_bodyType = other._bodyType;
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
		this->_supportedMethods = rhs._supportedMethods;
		this->_isCGI = rhs._isCGI;
		this->_contentLength = rhs._contentLength;
		this->_bodyType = rhs._bodyType;
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
		if (pos == std::string::npos)
			return ;
		++_isHeader;
		if (this->receiveInfo() == 0)
			this->_isRecvFinish = true;
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

	begin = line.find_first_not_of(" \t");
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
			end = line.find_last_not_of(" \t\v\r\n\f", colon - 1);
			if (end == std::string::npos)
				return ;
			end += 1;
			header_name = line.substr(0, end);
			for (size_t	i = 0; i < header_name.length(); ++i)
				header_name[i] = tolower(header_name[i]);
			this->_lastHeader = header_name;
			line = line.substr(colon + 1, line.length() - (colon + 1));
			begin = line.find_first_not_of(" \t\v\r\n\f");
			end = line.find_last_not_of(" \t\v\r\n\f", line.length() - 1);
			if (begin == std::string::npos || end == std::string::npos)
			{
				if (this->_header.find(header_name) == this->_header.end())
					this->_header.insert(std::make_pair(header_name, ""));
				return ;
			}
			end += 1;
			header_value = line.substr(begin, end - begin);
			if (this->_header.find(header_name) == this->_header.end())
				this->_header.insert(std::make_pair(header_name, header_value));
			else if (this->_header[header_name].find(",") != std::string::npos)
				this->_header[header_name] += header_value;
		}
	}
}

int	Client::receiveInfo()
{
	std::map<std::string, std::string>::iterator	host;
	std::string::size_type							pos;

	if (std::find(_supportedMethods.begin(), _supportedMethods.end(), \
			_header.find("method")->second) == _supportedMethods.end())
		return (getError(501));
	host = this->_header.find("host");
	if (host == this->_header.end())
		return (getError(400));
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
		if (this->findServer() == 0)
			return (0);
		else
		{
			_file = this->_header.find("uri")->second;
			if (_file[0] != '/')
				return (getError(400));
			else
			{
				pos = _file.find_last_of("/.");
				if (_file[pos] == '/' && _file[_file.length() - 1] != '/')
					_file += "/";
				this->findLocation();
				if (this->isAllowedMethods() == false)
					return (getError(405));
				if (this->findFile(_file, pos) == false)
					return (getError(404));
				this->findLength();
				this->findCgi();
			}
		}
	}
	return (1);
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

void	Client::findLocation()
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
													_file.begin()) == 1)
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
		findLocation();
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
			tmp = full_path + *it_begin;
			if (access(tmp.c_str(), F_OK | R_OK) == 0)
			{
				full_path = tmp;
				return (true);
			}
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
		tmp = full_path;
		if (access(tmp.c_str(), F_OK | R_OK) == 0)
		{
			full_path = tmp;
			return (true);
		}
	}
	return (false);
}

void	Client::findLength()
{
	std::map<t_str, t_str>::iterator	it;

	it = this->_header.find("Transfer-Encoding");
	if (it != this->_header.end())
	{
		for (size_t	i = 0; i < it->second.length(); ++i)
			it->second[i] = tolower(it->second[i]);
		if (it->second == "chunked")
			this->_bodyType = "chunked";
	}
	else
	{
		it = this->_header.find("Content-Length");
		if (it != this->_header.end())
		{
			this->_bodyType = "length";
			this->_contentLength = std::strtoul(it->second.c_str(), NULL, 0);
		}
	}
}

void	Client::findCgi()
{
	std::map<t_str, t_str>::const_iterator	it_begin;
	std::map<t_str, t_str>::const_iterator	it_end;
	std::string::size_type					pos;

	pos = this->_file.find_last_of("/");
	pos = this->_file.find_first_of(".", pos);
	if (pos != std::string::npos)
	{
		if (this->_isLocation == true)
		{
			it_begin = this->_location.second.getCgi().begin();
			it_end = this->_location.second.getCgi().end();
		}
		else
		{
			it_begin = this->_server.getCgi().begin();
			it_end = this->_server.getCgi().end();
		}
		while (it_begin != it_end)
		{
			if (it_begin->first.compare(pos, it_begin->first.length(), \
										this->_file) == 0)
			{
				this->_isCGI = true;
				return ;
			}
			++it_begin;
		}
	}
	this->_isCGI = false;
}

bool	Client::isAllowedMethods()
{
	std::vector<t_str>::const_iterator	it_begin;
	std::vector<t_str>::const_iterator	it_end;
	std::vector<t_str>::const_iterator	it_begin_tmp;

	if (this->_isLocation == true)
	{
		it_begin = this->_location.second.getAllowedMethods().begin();
		it_end = this->_location.second.getAllowedMethods().end();
	}
	else
	{
		it_begin = this->_server.getAllowedMethods().begin();
		it_end = this->_server.getAllowedMethods().end();
	}
	it_begin_tmp = it_begin;
	while (it_begin != it_end)
	{
		if (*it_begin == this->_header["method"])
			return (true);
		++it_begin;
	}
	it_begin = it_begin_tmp;
	this->_errorAllowed = "Allow: ";
	while (it_begin != it_end)
	{
		this->_errorAllowed += (*it_begin + ", ");
		++it_begin;
	}
	this->_errorAllowed = _errorAllowed.substr(0, _errorAllowed.length() - 2);
	return (false);
}

void	Client::parsingBody()
{
	if (this->_isRecvFinish == false)
		this->readBody();
	if (this->_isRecvFinish == true)
		this->prepareAnswer();
}

void	Client::readBody()
{
	if (_bodyType == "chunked")
	{
	}
	else if (_bodyType == "length")
	{
		if (this->_request.length() >= this->_contentLength)
		{
			this->_contentLength = 0;
			this->_isRecvFinish = true;
		}
		else
			this->_contentLength -= this->_request.length();
		this->_body += this->_request;
		this->_request = "";
	}
	else
	{
		if (this->_request != "")
			getError(411);
		this->_isRecvFinish = true;
	}
}

void	Client::prepareAnswer()
{
	std::string				response;
	std::string				type;
	std::string::size_type	pos;
	std::stringstream		ss;

/*	if (this->_body.length() > _contentLength)
		getError(400);
	else
	{
	}*/
	if (this->_response == "")
	{
		if (this->_isCGI == true)
			Cgi(*this);
		else
		{
			pos = _file.find_last_of(".");
			this->readWhole(_file, _response);
			response += "HTTP/1.1 200 OK\r\n";
			if (pos == std::string::npos)
				response += "Content-Type : text/plain;\r\n";
			else
			{
				++pos;
				type = _file.substr(pos, _file.length() - pos);
				response += "Content-Type : text/" + type + ";\r\n";
			}
			ss << _response.length();
			response += "Content-Length : " + ss.str() + "\r\n";
			response += "Server : webserv\r\n";
			response += "\r\n";
			_response = response + _response;
		}
	}
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

int	Client::getError(int num)
{
	switch (num)
	{
		case 400:
			getErrorMsg(400, "400", "Bad Request");
			break ;
		case 404:
			getErrorMsg(404, "404", "Not Found");
			break ;
		case 405:
			getErrorMsg(405, "405", "Method Not Allowed");
			break ;
		case 411:
			getErrorMsg(411, "411", "Length Required");
			break ;
		case 501:
			getErrorMsg(501, "501", "Not Implemented");
			break ;
		default:
			return (0);
	}
	return (0);
}

bool	Client::responseErrorPage(int errNum, std::string &response_body) const
{
	std::map<int, std::string>::const_iterator	it;
	std::string									full_path;
	std::string									root;

	if (this->_isLocation == true)
	{
		it = this->_location.second.getErrorPage().find(errNum);
		if (it != this->_location.second.getErrorPage().end())
		{
			root = this->_location.second.getRoot();
			if (root[root.length() - 1] == '/'  && it->second[0] == '/')
				full_path = root + it->second.substr(1, it->second.length() - 1);
			else if (root[root.length() - 1] != '/'  && it->second[0] != '/')
				full_path = root + "/" + it->second;
			else
				full_path = root + it->second;
			if (access(full_path.c_str(), F_OK | R_OK) == 0)
				return (readWhole(full_path, response_body));
		}
	}
	it = this->_server.getErrorPage().find(errNum);
	if (it == this->_server.getErrorPage().end())
		return (false);
	root = this->_server.getRoot();
	if (root[root.length() - 1] == '/'  && it->second[0] == '/')
		full_path = root + it->second.substr(1, it->second.length() - 1);
	else if (root[root.length() - 1] != '/'  && it->second[0] != '/')
		full_path = root + "/" + it->second;
	else
		full_path = root + it->second;
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
	if (errNum == 405)
		response += this->_errorAllowed + "\r\n";
	response += "Server : webserv\r\n";
	response += "\r\n";
	response += response_body;
	this->_response = response;
}
