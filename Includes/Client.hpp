/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 16:38:11 by vismaily          #+#    #+#             */
/*   Updated: 2022/11/08 13:09:35 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>
# include <cstring>
# include <map>

class	Client
{
	public:
		Client();
		Client(const Client &other);
		Client	&operator=(const Client &rhs);
		~Client();

	public:
		void			setStr(const std::string &request);
		bool			getStatus() const;
	private:
		void			parsing();
		void			parsingRequestLine(std::string line);
		void			parsingHeader(std::string line);
		void			parsingBody();
	private:
		std::string							_request;
		std::map<std::string, std::string>	_header;
		std::string							_body;
		bool								_isFinish;

	private:
		int									_isStart;
		int									_isHeader;
		std::string							_lastHeader;
};

#endif
