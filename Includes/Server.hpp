/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 16:36:35 by vismaily          #+#    #+#             */
/*   Updated: 2022/10/29 16:43:22 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

class	Server
{
	public:
		Server();
		Server(const Server &other);
		Server	&operator=(const Server &rhs);
		~Server();
	private:
};

#endif
