/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 16:38:11 by vismaily          #+#    #+#             */
/*   Updated: 2022/11/06 16:54:26 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <string>

class	Client
{
	public:
		Client();
		Client(const Client &other);
		Client	&operator=(const Client &rhs);
		~Client();

	public:
		void			setStr(const std::string &request);
	private:
		std::string		_request;
};

#endif
