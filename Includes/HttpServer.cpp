#include "HttpServer.hpp"

HttpServer::HttpServer()
{

}

HttpServer::~HttpServer()
{

}

HttpServer::HttpServer(const HttpServer &other)
{
	*this = other;
}

HttpServer &HttpServer::operator=(const HttpServer &other)
{
	if (this != &other)
	{
		this->acceptfds = other.acceptfds;
		this->listenSockets = other.listenSockets;
	}
	return (*this);
}

HttpServer::HttpServer(std::vector<Server> *vec)
{
	this->vec = *vec;
}

void HttpServer::createSockets(int i)
{
    struct sockaddr_in	address;
	int					on = 1;

	this->listenSockets[i].sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->listenSockets[i].sockfd < 0)
		throw std::runtime_error("Error: when creating a socket.");
	if (setsockopt(this->listenSockets[i].sockfd, SOL_SOCKET, \
				SO_REUSEADDR | SO_REUSEPORT, &on, sizeof(on)) < 0)
	{
		close(this->listenSockets[i].sockfd);
		throw std::runtime_error("Error: setsockopt");
	}
	memset((char *)&address, 0, sizeof(address));
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(this->listenSockets[i].host.c_str());
	address.sin_port = htons(this->listenSockets[i].port);
	if (bind(this->listenSockets[i].sockfd, (struct sockaddr *) &address, \
					sizeof(address)) < 0)
		throw std::runtime_error("Error: binding socket.");
	if (listen(this->listenSockets[i].sockfd, 32) < 0)
		throw std::runtime_error("Error: listening socket.");
}

void HttpServer::createListen()
{
   /* struct sockaddr_in address;
    socklen_t addrlen;*/
	int	c;

    c = 0;
    for (size_t i = 0; i < this->vec.size(); ++i)
    {
		std::map<std::string, std::string>::const_iterator it = vec[i].getListen().begin();
		while (it != vec[i].getListen().end())
        {
            this->listenSockets[c].host = it->first;
            this->listenSockets[c].port = atoi(it->second.c_str());
            createSockets(c);
			++c;
            ++it;
        }
    }
}

void HttpServer::createacceptfd(int i, fd_set initrset, int *maxfd)
{
	struct sockaddr_in	address;
	int					fd;
	socklen_t			addrlen;

	if ((fd = accept(this->listenSockets[i].sockfd, (struct sockaddr *)&address, \
					(socklen_t*)&addrlen)) < 0)
		throw std::runtime_error("Error: accept");
	fcntl(fd, F_SETFL, O_NONBLOCK);
	FD_SET(fd, &initrset);
	(*maxfd)++;
	this->acceptfds.push_back(fd);
}

void HttpServer::getrequest(int i)
{
    char	buffer[1024];
    int		n = 0;

	if ((n = recv(this->acceptfds[i], buffer, sizeof(buffer) - 1, 0)) < 0)
		throw std::runtime_error("Error: recieve");
	buffer[n] = '\0';
	std::cout << buffer << std::endl;
}


void HttpServer::run()
{
    fd_set readset, writeset, initwset, initrset;
    int maxfd;

    createListen();
    maxfd = 0; 
    FD_ZERO(&initrset);
    FD_ZERO(&initwset);
    for (size_t i = 0; i < this->listenSockets.size(); i++)
    {
        if (this->listenSockets[i].sockfd > maxfd)
            maxfd = listenSockets[i].sockfd;
        FD_SET(this->listenSockets[i].sockfd, &initrset);
    }
    while (1)
    {
        readset = initrset;
        writeset = initwset;
        select(maxfd, &readset, &writeset, 0, 0);
        for (size_t i = 0; i < this->listenSockets.size(); i++)
        {
            if (FD_ISSET(this->listenSockets[i].sockfd, &readset) == 1)
            {
                createacceptfd(i, initrset,  &maxfd);
            }
        }
        for (size_t i = 0; i < this->acceptfds.size(); i++)
        {
            if (FD_ISSET(this->acceptfds[i], &readset) == 1)
            {
                getrequest(i);
                FD_SET(this->acceptfds[i], &initwset);
            }
            if (FD_ISSET(this->acceptfds[i], &readset) == 1)
            {
               // sendresponse(i);
            }
        }
    }
}
