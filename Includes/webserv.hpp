/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   webserv.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/28 12:50:24 by vismaily          #+#    #+#             */
/*   Updated: 2022/10/28 14:56:30 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef WEBSERV_HPP
# define WEBSERV_HPP

# define CONFIG_DEFAULT		"default.conf"
# define CONFIG_PATH		"./Configs/"
# define CONFIG_EXTANSION	".conf"

# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>
# include <iostream>
# include <fstream>
# include <cstring>

int	file_open(std::string config_file);

#endif
