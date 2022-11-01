/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 16:36:35 by vismaily          #+#    #+#             */
/*   Updated: 2022/11/01 18:19:18 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <vector>
# include <cstring>

# include <iostream>

class	Server
{
	private:
		typedef std::string			str_t;
	public:
		Server(str_t &body);
		Server(const Server &other);
		Server	&operator=(const Server &rhs);
		~Server();
	private:
		Server();

	public:
		void							setServerName(str_t &value);
		const std::vector<std::string>	&getServerName() const;
	private:
		void		setFildes(const str_t &name, str_t &value);
		void		parsingValue(str_t &body, str_t::size_type value_begin, \
								 str_t::size_type value_end);
		void		parsingBody(str_t &body);
	private:
		std::vector<str_t>			_directive_list;
		std::vector<str_t>			_serverName;
};

#endif
