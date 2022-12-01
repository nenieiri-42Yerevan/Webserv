/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   delete_comments.cpp                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 12:36:02 by vismaily          #+#    #+#             */
/*   Updated: 2022/12/01 13:32:43 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	delete_comments(std::string &config)
{
	std::string				tmp;
	std::string::size_type	pos;
	std::string::size_type	search_pos;

	search_pos = 0;
	tmp = config;
	config = "";
	while (tmp != "" && tmp[0] == '#')
	{
		pos = tmp.find_first_of("\n");
		if (pos == std::string::npos)
		{
			tmp = "";
			break ;
		}
		++pos;
		tmp = tmp.substr(pos, tmp.length() - pos);
	}
	while (tmp != "")
	{
		pos = tmp.find_first_of(" \n\r\t\v\f", search_pos);
		if (pos == std::string::npos)
		{
			config += tmp;
			break ;
		}
		if (tmp[pos + 1] == '#')
		{
			++pos;
			config += tmp.substr(0, pos);
			pos = tmp.find_first_of("\n", pos);
			if (pos == std::string::npos)
				break ;
			tmp = tmp.substr(pos, tmp.length() - pos);
			search_pos = 0;
		}
		else
			search_pos = pos + 1;
	}
}
