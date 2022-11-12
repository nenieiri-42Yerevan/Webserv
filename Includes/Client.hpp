/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/06 16:38:11 by vismaily          #+#    #+#             */
/*   Updated: 2022/11/12 16:06:18 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>
# include <cstring>
# include <sstream>
# include <map>
# include <vector>

# include "Server.hpp"

class	Client
{
	private:
		typedef std::string		t_str;
	public:
		Client(std::vector<Server> &serverSet);
		Client(const Client &other);
		Client	&operator=(const Client &rhs);
		~Client();
	private:
		Client();

	public:
		void					setRequest(const std::string &request);
		bool					getRecvStatus() const;
		bool					getSendStatus() const;
		const std::string		getResponse();
		const std::string		getResponse(size_t	buff_size);
	private:
		void					parsing();
		int						parsingRequestLine(std::string line);
		void					parsingHeader(std::string line);
		void					parsingBody();
		void					prepareAnswer();
		void					findServer();
		std::string				getError(int num);
		std::string				getErrorMsg(const t_str &num, const t_str &msg);
	private:
		std::string				_request;
		std::string				_response;
		std::map<t_str, t_str>	_header;
		std::string				_body;
		bool					_isRecvFinish;
		bool					_isSendFinish;
		std::string				_version;
		int						_isStart;
		int						_isHeader;
		std::string				_lastHeader;
		std::vector<Server>		_serverSet;
		std::string				_port;
		std::string				_host;
		int						_serverNumber;
};

#endif
