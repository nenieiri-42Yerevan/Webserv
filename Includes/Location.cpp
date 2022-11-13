/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 12:20:56 by vismaily          #+#    #+#             */
/*   Updated: 2022/11/13 16:19:40 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Location.hpp"

/*=====================================*/
/*     Constructors and Destructor     */
/*=====================================*/

Location::Location()
{
}

Location::Location(t_str &body)
{
	_directiveList.push_back("root");
	_directiveList.push_back("index");
	_directiveList.push_back("autoindex");
	_directiveList.push_back("error_page");
	_directiveList.push_back("location");
	this->_autoindex = false;
	parsingBody(body);
}

Location::Location(const Location &other)
{
	this->_root = other._root;
	this->_index = other._index;
	this->_autoindex = other._autoindex;
	this->_location = other._location;
	this->_errorPage = other._errorPage;
}

Location	&Location::operator=(const Location &rhs)
{
	if (this != &rhs)
	{
		this->_root = rhs._root;
		this->_index = rhs._index;
		this->_autoindex = rhs._autoindex;
		this->_location = rhs._location;
		this->_errorPage = rhs._errorPage;
	}
	return (*this);
}

Location::~Location()
{
}

/*=====================================*/
/*               Getters               */
/*=====================================*/

const std::string	&Location::getRoot() const
{
	return (this->_root);
}

const std::map<std::string, Location>	&Location::getLocation() const
{
	return (this->_location);
}

const std::vector<std::string>	&Location::getIndex() const
{
	return (this->_index);
}

bool	Location::getAutoindex() const
{
	return (this->_autoindex);
}

const std::map<int, std::string>	&Location::getErrorPage() const
{
	return (this->_errorPage);
}

/*=====================================*/
/*               Setters               */
/*=====================================*/

void	Location::setRoot(t_str &value)
{
	std::string::size_type	pos;

	pos = value.find_last_not_of(" \t\v\r\n\f");
	if (pos == std::string::npos)
		throw std::runtime_error("Error: Root value is empty.");
	++pos;
	value = value.substr(0, pos);
	if (value.find_first_of(" \t\v\r\n\f") != std::string::npos)
		throw std::runtime_error("Error: Root isn't valid "
								 "(there are whitespaces).");
	this->_root = value;
}

void	Location::setLocation(t_str &value)
{
	std::string::size_type	pos;
	std::string::size_type	pos2;
	std::string				name;
	std::string				inner;

	pos = value.find_first_of("{");
	name = value.substr(0, pos);
	pos2 = name.find_last_not_of(" \t\v\r\n\f");
	if (pos2 == std::string::npos)
		throw std::runtime_error("Error: Location is not valid.");
	++pos2;
	name = name.substr(0, pos2);
	++pos;
	inner = value.substr(pos, value.length() - 1 - pos);
	this->_location.insert(std::make_pair(name, Location(inner)));
}

void	Location::setIndex(t_str &value)
{
	char	*token;

	token = std::strtok(&value[0], " \t\v\r\n\f");
	if (token == NULL)
		throw std::runtime_error("Error: Config file: Directive "
								 "value of index is empty.");
	while (token != NULL)
	{
		this->_index.push_back(token);
		token = std::strtok(NULL, " \t\v\r\n\f");
	}
}

void	Location::setAutoindex(t_str &value)
{
	std::string::size_type	pos;

	pos = value.find_last_not_of(" \t\v\r\n\f");
	if (pos == std::string::npos)
		throw std::runtime_error("Error: Autoindex value is empty.");
	++pos;
	value = value.substr(0, pos);
	if (value == "on")
		this->_autoindex = true;
	else if (value == "off")
		this->_autoindex = false;
	else
		throw std::runtime_error("Error: Autoindex value can only be "
								 "'on' or 'off'.");
}

void	Location::setErrorPage(t_str &value)
{
	t_str::size_type	pos;
	char				*token;
	t_str				page;
	int					error_number;

	pos = value.find_last_not_of(" \t\v\r\n\f");
	if (pos == std::string::npos)
		throw std::runtime_error("Error: Config file: Directive "
								 "value of error_page is empty.");
	value = value.substr(0, ++pos);
	pos = value.find_last_of(" \t\v\r\n\f");
	if (pos == std::string::npos)
		throw std::runtime_error("Error: Config file: Directive value of "
								 "error_page must contain an error number"
								 "and after that an error page uri.");
	++pos;
	page = value.substr(pos, value.length() - pos);
	value = value.substr(0, pos - 1);
	token = std::strtok(&value[0], " \t\v\r\n\f");
	while (token != NULL)
	{
		error_number = std::atoi(token);
		if (!(error_number >= 300 && error_number <= 599))
			throw std::runtime_error("Error: Config file: Directive "
									 "value of error_page must be between "
									 "'300' and '599' range.");
		this->_errorPage.insert(std::make_pair(error_number, page));
		token = std::strtok(NULL, " \t\v\r\n\f");
	}
}

void	Location::setFildes(const t_str &name, t_str &value)
{
	if (name.compare("root") == 0)
		this->setRoot(value);
	else if (name.compare("location") == 0)
		this->setLocation(value);
	else if (name.compare("index") == 0)
		this->setIndex(value);
	else if (name.compare("autoindex") == 0)
		this->setAutoindex(value);
	else if (name.compare("error_page") == 0)
		this->setErrorPage(value);
}

/*=====================================*/
/*       Other Member Functions        */
/*=====================================*/

void	Location::parsingLocation(t_str &body, t_str::size_type &value_begin, \
							 t_str::size_type &value_end)
{
	t_str::size_type	count = 1;

	value_end = body.find_first_of("{}", value_begin);
	if (value_end == t_str::npos || body[value_end] == '}')
		throw std::runtime_error("Error: Config file: directive " \
								 "'location' has no opening '{'.");
	value_end = body.find_first_of("{}", value_end + 1);
	while (value_end != t_str::npos && count != 0)
	{
		if (body[value_end] == '{')
			++count;
		else
			--count;
		if (count != 0)
			value_end = body.find_first_of("{}", value_end + 1);
	}
	if (value_end == t_str::npos)
		throw std::runtime_error("Error: Config file: directive " \
								 "'location' has no closing '}'.");
	++value_end;
}

void	Location::parsingBody(t_str &body)
{
	t_str::size_type	name_begin;
	t_str::size_type	name_end;
	t_str				name;
	t_str::size_type	value_begin;
	t_str::size_type	value_end;
	t_str				value;

	while (body != "")
	{
		name_begin = body.find_first_not_of(" \t\v\r\n\f");
		if (name_begin == t_str::npos)
			break ;
		name_end = body.find_first_of(" \t\v\r\n\f", name_begin);
		if (name_end == t_str::npos)
			throw std::logic_error("Error: Config file: after directive name " \
								   "must be at least one whitespce and " \
								   "after that must be its value.");
		name = body.substr(name_begin, name_end - name_begin);
		if (std::find(_directiveList.begin(), _directiveList.end(), name) == \
													_directiveList.end())
			throw std::logic_error("Error: Config file: directive name '" \
				   					+ name + "' unknown.");
		value_begin = body.find_first_not_of(" \t\v\r\n\f", name_end);
		if (value_begin == t_str::npos)
			throw std::logic_error("Error: Config file: directive '" + name + \
									"' doesn't have a value.");
		if (body.compare(name_begin, 8, "location") != 0)
		{
			value_end = body.find_first_of(';', value_begin);
			if (value_end == t_str::npos)
				throw std::logic_error("Error: Config file: value of "
					"directive '" + name + "' must end with ';' symbol.");
		}
		else
			parsingLocation(body, value_begin, value_end);
		value = body.substr(value_begin, value_end - value_begin);
		this->setFildes(name, value);
		if (body.compare(name_begin, 8, "location") != 0)
			body.erase(0, value_end + 1);
		else
			body.erase(0, value_end);
	}
}
