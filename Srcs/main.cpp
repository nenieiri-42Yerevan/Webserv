/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vismaily <nenie_iri@mail.ru>               +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/25 13:17:05 by vismaily          #+#    #+#             */
/*   Updated: 2022/10/26 11:51:50 by vismaily         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iostream>

int	main(int argc, char **argv)
{
	int			server_socket;

	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket < 0)
		std::cout << "Error establishing connection." << std::endl;
	else
	{
		sockaddr_in	server_addr;
		server_addr.sin_family = AF_INET;
		server_addr.sin_port = htons(80);
		server_addr.sin_addr.s_addr = INADDR_ANY;
		if (bind(server_socket, (sockaddr *)&server_addr, sizeof(server_addr)) == -1)
		{
			std::cout << "Error binding socket." << std::endl;
			return (0);
		}
		if (listen(server_socket, 5) == -1)
		{
			std::cout << "Error listening socket." << std::endl;
			return (0);
		}
		sockaddr_in	client_addr;
		socklen_t	client_size = sizeof(client_addr);
		int	client_socket;
		client_socket = accept(server_socket, (sockaddr *)&client_addr, &client_size);
		if (a == -1)
		{
			std::cout << "Error on accepting." << a << std::endl;
			return (0);
		}
	}
	return (0);
}
