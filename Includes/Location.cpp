/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Location.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/09 12:20:56 by vismaily          #+#    #+#             */
/*   Updated: 2022/11/09 12:51:52 by vismaily         ###   ########.fr       */
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
	_directiveList.push_back("location");
	parsingBody(body);
}

Location::Location(const Location &other)
{
	this->_root = other._root;
}

Location	&Location::operator=(const Location &rhs)
{
	if (this != &rhs)
	{
		this->_root = rhs._root;
	}
	return (*this);
}

Location::~Location()
{
}

/*=====================================*/
/*          Setter and Getters         */
/*=====================================*/

void	Location::setRoot(const t_str &root)
{
	this->_root = root;
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

void	Location::setFildes(const t_str &name, t_str &value)
{
	if (name.compare("root") == 0)
		this->parsingRoot(value);
	else if (name.compare("location") == 0)
		this->setLocation(value);
}

/*=====================================*/
/*       Other Member Functions        */
/*=====================================*/

void	Location::parsingRoot(t_str &value)
{
	std::string::size_type	pos;

	pos = value.find_last_not_of(" \t\v\r\n\f");
	if (pos == std::string::npos)
		throw std::runtime_error("Error: Root is not valid.");
	++pos;
	value = value.substr(0, pos);
	setRoot(value);
}

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
