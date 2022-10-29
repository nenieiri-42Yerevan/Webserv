/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 16:42:16 by vismaily          #+#    #+#             */
/*   Updated: 2022/10/29 16:58:33 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Server.hpp"

Server::Server()
{
}

Server::Server(const Server &other)
{
	(void)other;
}

Server	&Server::operator=(const Server &rhs)
{
	if (this != &rhs)
	{
	}
	return (*this);
}

Server::~Server()
{
}
