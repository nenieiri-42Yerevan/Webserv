#ifndef HTTPSERVER_HPP
# define HTTPSERVER_HPP

# include <vector>
# include <map>
# include <cstring>
# include "webserv.hpp"

typedef struct s_serv
{
	std::string	host;
	int			sockfd;
	int			port;
}				t_serv;

class	HttpServer
{
	public:
		HttpServer();
		HttpServer(std::vector<Server> *vec);
		HttpServer(const HttpServer &other);
		HttpServer			&operator=(const HttpServer &other);
		virtual				~HttpServer();

	public:
		void				createListen();
		void				createSockets(int i);
	private:
		std::vector<t_serv>	clientSockets;
		std::vector<t_serv>	listenSockets;
		std::vector<Server>	vec;
};

#endif
