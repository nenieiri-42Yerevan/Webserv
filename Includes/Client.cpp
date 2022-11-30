/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 16:38:07 by vismaily          #+#    #+#             */
/*   Updated: 2022/11/30 17:30:35 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Client.hpp"
#include "Cgi.hpp"

/*=====================================*/
/*     Constructors and Destructor     */
/*=====================================*/

Client::Client()
{
	this->_request = "";
	this->_body = "";
	this->_chunkedBody = "";
	this->_chunkedLen = -1;
	this->_isRecvFinish = false;
	this->_isSendFinish = false;
	this->_version = "webserv/1.0.0";
	this->_isStart = 0;
	this->_isHeader = 0;
	this->_lastHeader = "";
	this->_isServerFound = false;
	this->_isLocation = false;
	this->_contentLength = 0;
	this->_isCgi = false;
	this->_isClosed = false;
	this->_closeTime = time(NULL);
	this->_supportedMethods.push_back("GET");
	this->_supportedMethods.push_back("POST");
	this->_supportedMethods.push_back("DELETE");
}

Client::Client(std::vector<Server> &serverSet, int serverNumber)
{
	this->_request = "";
	this->_body = "";
	this->_chunkedBody = "";
	this->_chunkedLen = -1;
	this->_isRecvFinish = false;
	this->_isSendFinish = false;
	this->_version = "webserv/1.0.0";
	this->_isStart = 0;
	this->_isHeader = 0;
	this->_lastHeader = "";
	this->_isServerFound = false;
	this->_serverSet = serverSet;
	this->_server = serverSet[serverNumber];
	this->_isLocation = false;
	this->_contentLength = 0;
	this->_isCgi = false;
	this->_isClosed = false;
	this->_closeTime = time(NULL);
	this->_supportedMethods.push_back("GET");
	this->_supportedMethods.push_back("POST");
	this->_supportedMethods.push_back("DELETE");
}

Client::Client(const Client &other)
{
	this->_request = other._request;
	this->_response = other._response;
	this->_header = other._header;
	this->_file = other._file;
	this->_body = other._body;
	this->_chunkedBody = other._chunkedBody;
	this->_chunkedLen = other._chunkedLen;
	this->_isRecvFinish = other._isRecvFinish;
	this->_isSendFinish = other._isSendFinish;
	this->_version = other._version;
	this->_isStart = other._isStart;
	this->_isHeader = other._isHeader;
	this->_lastHeader = other._lastHeader;
	this->_isServerFound = other._isServerFound;
	this->_serverSet = other._serverSet;
	this->_server= other._server;
	this->_isLocation = other._isLocation;
	this->_supportedMethods = other._supportedMethods;
	this->_errorAllowed = other._errorAllowed;
	this->_bodyType = other._bodyType;
	this->_contentLength = other._contentLength;
	this->_isCgi = other._isCgi;
	this->_Cgi = other._Cgi;
	this->_uploadDir = other._uploadDir;
	this->_isClosed = other._isClosed;
	this->_closeTime = other._closeTime;
	this->_redirectHeader = other._redirectHeader;
}

Client	&Client::operator=(const Client &rhs)
{
	if (this != &rhs)
	{
		this->_request = rhs._request;
		this->_response = rhs._response;
		this->_header = rhs._header;
		this->_file = rhs._file;
		this->_body = rhs._body;
		this->_chunkedBody = rhs._chunkedBody;
		this->_chunkedLen = rhs._chunkedLen;
		this->_isRecvFinish = rhs._isRecvFinish;
		this->_isSendFinish = rhs._isSendFinish;
		this->_version = rhs._version;
		this->_isStart = rhs._isStart;
		this->_isHeader = rhs._isHeader;
		this->_lastHeader = rhs._lastHeader;
		this->_isServerFound = rhs._isServerFound;
		this->_serverSet = rhs._serverSet;
		this->_server= rhs._server;
		this->_isLocation = rhs._isLocation;
		this->_supportedMethods = rhs._supportedMethods;
		this->_errorAllowed = rhs._errorAllowed;
		this->_bodyType = rhs._bodyType;
		this->_contentLength = rhs._contentLength;
		this->_isCgi = rhs._isCgi;
		this->_Cgi = rhs._Cgi;
		this->_uploadDir = rhs._uploadDir;
		this->_isClosed = rhs._isClosed;
		this->_closeTime = rhs._closeTime;
		this->_redirectHeader = rhs._redirectHeader;
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
	this->_isSendFinish = false;
	this->_isClosed = false;
	this->_closeTime = time(NULL);
	parsing();
}

void	Client::setResponse(const std::string &response)
{
	this->_response += response;
}

bool	Client::getRecvStatus() const
{
	return (this->_isRecvFinish);
}

bool	Client::getSendStatus() const
{
	return (this->_isSendFinish);
}

bool	Client::getCloseStatus()
{
	if (this->_isSendFinish == true && \
			time(NULL) - this->_closeTime > CONNECTION_CLOSE_SECONDS)
		this->_isClosed = true;
	return (this->_isClosed);
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

const std::string	&Client::getFile() const
{
	return (this->_file);
}

const std::map<std::string, std::string>	Client::getHeader() const
{
	return (this->_header);
}

const std::pair<std::string, std::string>	Client::getCgi() const
{
	return (this->_Cgi);
}

const std::string	&Client::getUploadDir() const
{
	return (this->_uploadDir);
}

const std::string	&Client::getBody() const
{
	return (this->_body);
}

const std::string	&Client::getServerPort() const
{
	return (this->_port);
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
	std::map<std::string, std::string>::iterator	connection;
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
		connection = this->_header.find("connection");
		if (connection != this->_header.end())
		{
			if (connection->second == "close")
				this->_isClosed = true;
			else
				this->_closeTime = time(NULL);
		}
		if (this->_isServerFound == false && this->findServer() == 0)
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
				if (this->findReturn() == true)
					return (0);
				if (this->findFile(_file, pos) == false)
				{
					if (this->_response == "")
						return (getError(404));
					else
						return (0);
				}
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
				this->_isServerFound = true;
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
				this->_isServerFound = true;
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

bool	Client::findReturn()
{
	std::string	ret;

	if (this->_isLocation == true)
		ret = this->_location.second.getReturn();
	else
		ret = this->_server.getReturn();
	if (ret != "")
	{
		this->_redirectHeader = "Location : " + ret;
		getError(301);
		return (true);
	}
	return (false);
}

bool	Client::findFile(std::string &full_path, std::string::size_type pos)
{
	std::vector<t_str>::const_iterator	it_begin;
	std::vector<t_str>::const_iterator	it_end;
	std::string							tmp;
	std::string							root;
	std::string							rel_path(full_path);
	std::string::size_type				len;
	std::string::size_type				pos_post;
	bool								autoindex = false;

	if (full_path[pos] == '/')
	{
		if (this->_isLocation == true)
		{
			len = this->_location.first.length();
			full_path = full_path.substr(len, full_path.length() - len);
			full_path = this->_location.second.getRoot() + full_path;
			it_begin = this->_location.second.getIndex().begin();
			it_end = this->_location.second.getIndex().end();
			autoindex = this->_location.second.getAutoindex();
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
			autoindex = this->_server.getAutoindex();
		}
		for (; it_begin != it_end; ++it_begin)
		{
			if (full_path[full_path.length() - 1] == '/'  && (*it_begin)[0] == '/')
				tmp = full_path + it_begin->substr(1, it_begin->length() - 1);
			else if (full_path[full_path.length() - 1] != '/'  && (*it_begin)[0] != '/')
				tmp = full_path + "/" + *it_begin;
			else
				tmp = full_path + *it_begin;
			pos_post = tmp.find_last_of("/");
			pos_post = tmp.find_first_of("?", pos_post);
			if (pos_post != std::string::npos)
				tmp = tmp.substr(0, pos_post);
			if (access(tmp.c_str(), F_OK | R_OK) == 0)
			{
				full_path = tmp;
				return (true);
			}
		}
		if (autoindex == true)
			return (directListening(full_path, rel_path));
	}
	else
	{
		if (this->_isLocation == true)
		{
			len = this->_location.first.length();
			full_path = full_path.substr(len, full_path.length() - len);

			if (_location.second.getRoot()[_location.second.getRoot().length() - 1] \
					== '/' && full_path[0] == '/')
				full_path = this->_location.second.getRoot() + \
							full_path.substr(1, full_path.length() - 1);
			else if (_location.second.getRoot()[_location.second.getRoot().length() - 1] \
					!= '/' && full_path[0] != '/')
				full_path = this->_location.second.getRoot() + "/" + full_path;
			else
				full_path = this->_location.second.getRoot() + full_path;
		}
		else
		{
			if (this->_server.getRoot()[this->_server.getRoot().length() - 1] == '/')
				root = this->_server.getRoot().substr(0, \
						this->_server.getRoot().length() - 1);
			else
				root = this->_server.getRoot();
			if (full_path[0] != '/')
				full_path = root + "/" + full_path;
			else
				full_path = root + full_path;
		}
		pos_post = full_path.find_last_of("/");
		pos_post = full_path.find_first_of("?", pos_post);
		if (pos_post != std::string::npos)
			full_path = full_path.substr(0, pos_post);
		if (access(full_path.c_str(), F_OK | R_OK) == 0)
			return (true);
	}
	return (false);
}

void	Client::findLength()
{
	std::map<t_str, t_str>::iterator	it;

	it = this->_header.find("transfer-encoding");
	if (it != this->_header.end())
	{
		for (size_t	i = 0; i < it->second.length(); ++i)
			it->second[i] = tolower(it->second[i]);
		if (it->second == "chunked")
			this->_bodyType = "chunked";
	}
	else
	{
		it = this->_header.find("content-length");
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

	/* upload directory */
	if (this->_isLocation == true)
		this->_uploadDir = this->_location.second.getUploadDir();
	else
		this->_uploadDir = this->_server.getUploadDir();
	/* --------------------- */

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
			if (this->_file.compare(++pos, strlen(it_begin->first.c_str()), \
										it_begin->first) == 0)
			{
				this->_isCgi = true;
				this->_Cgi = *it_begin;
				return ;
			}
			++it_begin;
		}
	}
	this->_isCgi = false;
}

bool	Client::directListening(std::string &full_path, std::string &rel_path)
{
	DIR					*opened_dir;
	dirent				*dir_struct;
	std::string			table;
	std::string			response;
	std::string			name;
	std::string			tmp_path;
	std::stringstream	ss;
	struct stat			buf;
	struct tm			*timeinfo;
	char				time_buf[100];

	if (access(full_path.c_str(), F_OK) != 0)
	{
		getError(404);
		return (false);
	}
	opened_dir = opendir(full_path.c_str());
	if (access(full_path.c_str(), X_OK) != 0 || opened_dir == NULL)
	{
		getError(403);
		return (false);
	}
	table += "<!DOCTYPE html><html><head><title>";
	table += "Index of ";
	table += rel_path;
	table += "</title>";
	table += "<style>";
	table += ".box>* {flex: 33.33%;}";
	table += ".box {display: flex; flex-wrap: wrap; width: 75%;}";
	table += "</style></head>";
	table += "<body><h1>";
	table += "Index of ";
	table += rel_path;
	table += "</h1><hr><pre class=\"box\">";

	dir_struct = readdir(opened_dir);
	while (dir_struct != NULL)
	{
		name = dir_struct->d_name;
		if (!(name[0] == '.' && name != ".."))
		{
			tmp_path = full_path + name;
			table += "<a href=\"";
			table += name;
			if (dir_struct->d_type == DT_DIR)
				table += "/";
			table += "\">";
			table += name;
			table += "</a>";
			if (stat(tmp_path.c_str(), &buf) == 0)
			{
				table += "<span>";
				timeinfo = localtime(&(buf.st_mtime));
				strftime(time_buf, 100, "%d-%b-%Y %H:%S", timeinfo);
				table += time_buf;
				table += "</span><span>";
				if (dir_struct->d_type == DT_DIR)
					table += "-";
				else
				{
					ss.str("");
					ss << buf.st_size;
					table += ss.str();
				}
				table += "</span>";
			}
			table += "\n";
		}
		dir_struct = readdir(opened_dir);
	}
	table += "</pre><hr></body></html>";

	response += "HTTP/1.1 200 OK\r\n";
	response += "Content-Type : text/html;\r\n";
	ss.str("");
	ss << table.length();
	response += "Content-Length : " + ss.str() + "\r\n";
	response += "Server : webserv\r\n";
	response += "\r\n";
	_response += response + table;
	closedir(opened_dir);
	return (true);
}

bool	Client::isAllowedMethods()
{
	std::vector<t_str>::const_iterator	it_begin;
	std::vector<t_str>::const_iterator	it_end;
	std::vector<t_str>::const_iterator	it_begin_tmp;

	if (this->_isLocation == true) {
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
	std::string::size_type	pos;
	std::stringstream		ss;
	std::string				tmp;

	if (_bodyType == "chunked")
	{
		this->_chunkedBody += this->_request;
		this->_request = "";
		while (this->_isRecvFinish == false)
		{
			if (_chunkedLen == -1)
			{
				pos = _chunkedBody.find("\r\n");
				if (pos == std::string::npos)
					break ;
				tmp = _chunkedBody.substr(0, pos);
				pos += 2;
				if (_chunkedBody.length() < pos)
					break ;
				_chunkedBody = _chunkedBody.substr(pos, _chunkedBody.length() - pos);
				pos = tmp.find_first_of(":");
				if (pos != std::string::npos)
					tmp = tmp.substr(0, pos);
				ss << std::hex << tmp;
				ss >> _chunkedLen;
				if (_chunkedLen < 0)
					_chunkedLen = 0;
			}
			if (_chunkedLen == 0 || _chunkedLen == -10)
			{
				_chunkedLen = -10;
				pos = _chunkedBody.find("\r\n");
				if (pos == std::string::npos)
					break ;
				_chunkedBody = "";
				this->_isRecvFinish = true;
			}
			else if (_chunkedBody.length() >= static_cast<t_str::size_type>(_chunkedLen))
			{
				if (_chunkedBody.length() < (static_cast<t_str::size_type>(_chunkedLen) + 2))
					break ;
				this->_body += _chunkedBody.substr(0, _chunkedLen);
				_chunkedLen += 2;
				_chunkedBody = _chunkedBody.substr(_chunkedLen, \
										_chunkedBody.length() - _chunkedLen);
				_chunkedLen = -1;
				pos = _chunkedBody.find("\r\n");
			}
			else
				break ;
		}
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
	std::string				file_body;
	std::string				type;
	std::string::size_type	pos;
	std::stringstream		ss;
	unsigned long int		length;

	if (this->_isLocation == true)
		length = this->_location.second.getClientMaxBodySize();
	else
		length = this->_server.getClientMaxBodySize();
	if (length != 0 && this->_body.length() > length)
		getError(413);
	else
	{
		if (this->_isCgi == true)
		{
			Cgi	cgi(this);
			cgi.cgi_run();
		}
		else
		{
			pos = _file.find_last_of(".");
			this->readWhole(_file, file_body);
			response += "HTTP/1.1 200 OK\r\n";
			if (pos == std::string::npos)
				response += "Content-Type : text/plain;\r\n";
			else
			{
				++pos;
				type = _file.substr(pos, _file.length() - pos);
				response += "Content-Type : text/" + type + ";\r\n";
			}
			ss << file_body.length();
			response += "Content-Length : " + ss.str() + "\r\n";
			response += "Server : webserv\r\n";
			response += "\r\n";
			_response += response + file_body;
		}
	}
	resetHeader();
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
		case 301:
			getErrorMsg(301, "301", "Moved Permanently");
			break ;
		case 400:
			getErrorMsg(400, "400", "Bad Request");
			break ;
		case 403:
			getErrorMsg(403, "403", "Forbidden");
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
		case 413:
			getErrorMsg(413, "413", "Request Entity Too Large");
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
	if (errNum == 301)
		response += this->_redirectHeader + "\r\n";
	if (errNum == 405)
		response += this->_errorAllowed + "\r\n";
	response += "Server : webserv\r\n";
	response += "\r\n";
	response += response_body;
	this->_response += response;
}

void	Client::resetHeader()
{
	this->_request = "";
	this->_header.clear();
	this->_body = "";
	this->_chunkedBody = "";
	this->_chunkedLen = -1;
	this->_isRecvFinish = false;
	this->_isStart = 0;
	this->_isHeader = 0;
	this->_lastHeader = "";
	this->_isLocation = false;
	this->_errorAllowed = "";
	this->_bodyType = "";
	this->_contentLength = 0;
	this->_isCgi = false;
	this->_redirectHeader = "";
}
