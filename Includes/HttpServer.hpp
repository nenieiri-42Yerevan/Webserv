#ifndef HTTPSERVER_HPP
# define HTTPSERVER_HPP

# include <vector>
# include <map>
# include <string>
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
		int getrequest(int i);
		int sendresponse(int i);
	private:
		std::map<int, Client>	acceptfds;
		std::vector<t_serv>	listenSockets;
		std::vector<Server>	vec;
};

#endif
