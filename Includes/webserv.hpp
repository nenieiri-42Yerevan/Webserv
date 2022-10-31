/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 12:50:24 by vismaily          #+#    #+#             */
/*   Updated: 2022/10/31 12:32:28 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# define CONFIG_DEFAULT		"default.conf"
# define CONFIG_PATH		"./Configs/"
# define CONFIG_EXTANSION	".conf"

# include <iostream>
# include <fstream>
# include <sstream>
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

/* ------------- Common Part ---------------*/
void		file_open_read(std::string config_file, std::string &config);
void		start_server(std::string &config);

/* ------------- Parsing part --------------*/
std::vector<Server>	*parsing(std::string &config);
std::string			get_inside(std::string &config, std::string::size_type start_pos);

/*-------------- Server part ---------------*/
void createserv();
typedef struct  s_serv
{
    int maxfd;
    int sockfd;
    int acceptfd;
    int port;
    fd_set actual_set, read_set, write_set;
}               t_serv;


#endif
