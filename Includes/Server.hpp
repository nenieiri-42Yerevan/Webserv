/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 16:36:35 by vismaily          #+#    #+#             */
/*   Updated: 2022/10/31 13:01:24 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <vector>
# include <cstring>

# include <iostream>

class	Server
{
	public:
		Server();
		Server(std::string &body);
		Server(const Server &other);
		Server	&operator=(const Server &rhs);
		~Server();
	public:
		const std::vector<std::string>	getServerName() const;
	private:
		void							parsingBody(std::string &body);
	private:
		std::vector<std::string>		_serverName;
};

#endif
