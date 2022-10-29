/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 12:50:24 by vismaily          #+#    #+#             */
/*   Updated: 2022/10/29 15:26:31 by vismaily         ###   ########.fr       */
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

# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

int		file_open_read(std::string config_file, std::string &config);
void	start_server(std::string &config);

#endif
