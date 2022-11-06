/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   start_server.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/29 15:26:22 by vismaily          #+#    #+#             */
/*   Updated: 2022/11/06 13:13:42 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "webserv.hpp"

void	start_server(std::string &config)
{
	std::vector<Server>		*server_set;

	server_set = parsing(config);
	HttpServer	serv(server_set);
	serv.run();
	
	/*std::string	req = "POST /cgi-bin/process.cgi HTTP/1.1\n"
					"User-Agent: Mozilla/4.0 (compatible; MSIE5.01; Windows NT)\n"
					"Host: www.tutorialspoint.com\n"
					"Content-Type: application/x-www-form-urlencoded\n"
					"Content-Length: length\n"
					"Accept-Language: en-us"
					"Accept-Encoding: gzip, deflate\n"
					"Connection: Keep-Alive\n"
					"\n"
					"licenseID=string&content=string&/paramsXML=string\n";
	std::cout << req << std::endl;*/




	delete server_set;
}
