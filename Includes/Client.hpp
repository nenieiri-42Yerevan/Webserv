/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 16:38:11 by vismaily          #+#    #+#             */
/*   Updated: 2022/11/09 16:32:38 by vismaily         ###   ########.fr       */
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
		bool			getRecvStatus() const;
		bool			getSendStatus() const;
	private:
		void			parsing();
		void			parsingRequestLine(std::string line);
		void			parsingHeader(std::string line);
		void			parsingBody();
		void			prepareAnswer();
	private:
		std::string							_request;
		std::map<std::string, std::string>	_header;
		std::string							_body;
		bool								_isRecvFinish;
		bool								_isSendFinish;

	private:
		int									_isStart;
		int									_isHeader;
		std::string							_lastHeader;
};

#endif
