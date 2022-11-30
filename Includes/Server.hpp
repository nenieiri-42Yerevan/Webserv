/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 16:36:35 by vismaily          #+#    #+#             */
/*   Updated: 2022/11/30 15:02:33 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <vector>
# include <map>
# include <utility>
# include <string>
# include <cstring>
# include <cctype>
# include <cstdlib>
# include <algorithm>
# include <unistd.h>

# include "Location.hpp"

class	Server
{
	private:
		typedef std::string				t_str;

	public:
		Server();
		Server(t_str &body);
		Server(const Server &other);
		Server	&operator=(const Server &rhs);
		~Server();

	public:
		const std::vector<t_str>		&getServerName() const;
		const std::multimap<t_str, t_str>	&getListen() const;
		const std::string				&getRoot() const;
		const std::map<t_str, Location>	&getLocation() const;
		const std::vector<t_str>		&getIndex() const;
		bool							getAutoindex() const;
		const std::map<int, t_str>		&getErrorPage() const;
		unsigned long int				getClientMaxBodySize() const;
		const std::vector<t_str>		&getAllowedMethods() const;
		const std::map<t_str, t_str>	&getCgi() const;
		const std::string				&getUploadDir() const;
	private:
		void							setServerName(t_str &value);
		void							setLocation(t_str &value);
		void							setListen(t_str &addr, t_str &port);
		void							setRoot(t_str &value);
		void							setIndex(t_str &value);
		void							setAutoindex(t_str &value);
		void							setErrorPage(t_str &value);
		void							setClientMaxBodySize(t_str &value);
		void							setAllowedMethods(t_str &value);
		void							setCgi(t_str &value);
		void							setUploadDir(t_str &value);
		void							setReturn(t_str &value);
		void							setFildes(const t_str &name, t_str &value);
		void							setDefaults();
	private:
		void							parsingListen(t_str &value);
		bool							isValidPort(t_str port) const;
		bool							isValidIP(t_str addr) const;
		void							parsingLocation(t_str &body, \
											t_str::size_type &value_begin, \
											t_str::size_type &value_end);
		void							parsingBody(t_str &body);
	private:
		std::vector<t_str>				_directiveList;
		std::vector<t_str>				_serverName;
		std::multimap<t_str, t_str>		_listen;
		std::map<t_str, Location>		_location;
		std::string						_root;
		std::vector<t_str>				_index;
		bool							_autoindex;
		std::map<int, t_str>			_errorPage;
		unsigned long int				_clientMaxBodySize;
		std::vector<t_str>				_allowedMethods;
		std::map<t_str, t_str>			_cgi;
		std::string						_uploadDir;
		std::string						_return;
};

#endif
