/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 12:50:24 by vismaily          #+#    #+#             */
/*   Updated: 2022/11/11 17:55:48 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# define CONFIG_DEFAULT		"default.conf"
# define CONFIG_PATH		"./Configs/"
# define CONFIG_EXTANSION	".conf"
# define BUFREAD			1024
# define BUFWRITE			1024

# include <iostream>
# include <fstream>
# include <sstream>
# include <string>
# include <cstring>
# include <stdexcept>
# include <vector>
# include <sys/socket.h>
# include <sys/types.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <unistd.h>
# include <fcntl.h>

# include "Server.hpp"
# include "Client.hpp"
# include "HttpServer.hpp"

/* ------------- Common Part ---------------*/
void		file_open_read(std::string config_file, std::string &config);
void		start_server(std::string &config);

/* ------------- Parsing part --------------*/
std::vector<Server>	*parsing(std::string &config);
std::string			get_inside(std::string &config, std::string::size_type start_pos);

/*-------------- Server part ---------------*/

#endif
