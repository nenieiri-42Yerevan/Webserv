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
		void	createListen();
		void	createSockets(int i);
		void 	run();
		void createacceptfd(int i, fd_set *initset, int *maxfd);
		void getrequest(int i);
		void sendresponse(int i);
	private:
		std::vector<int>	acceptfds;
		std::vector<t_serv>	listenSockets;
		std::vector<Server>	vec;
};

#endif
