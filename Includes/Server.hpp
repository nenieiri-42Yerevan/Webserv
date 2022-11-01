/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 16:36:35 by vismaily          #+#    #+#             */
/*   Updated: 2022/11/01 19:12:28 by vismaily         ###   ########.fr       */
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
		typedef std::string			t_str;
	public:
		Server(t_str &body);
		Server(const Server &other);
		Server	&operator=(const Server &rhs);
		~Server();
	private:
		Server();

	public:
		void							setServerName(t_str &value);
		const std::vector<std::string>	&getServerName() const;
	private:
		void		setFildes(const t_str &name, t_str &value);
		void		parsingValue(t_str &body, t_str::size_type value_begin, \
								 t_str::size_type value_end);
		void		parsingBody(t_str &body);
	private:
		std::vector<t_str>			_directive_list;
		std::vector<t_str>			_serverName;
};

#endif
