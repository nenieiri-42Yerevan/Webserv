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
	socklen_t			on = 1;

	this->listenSockets[i].sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (this->listenSockets[i].sockfd < 0)
		throw std::runtime_error("Error: when creating a socket.");
	if (setsockopt(this->listenSockets[i].sockfd, SOL_SOCKET, \
				SO_REUSEADDR, &on, sizeof(on)) < 0)
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
    {
        close(this->listenSockets[i].sockfd);
		throw std::runtime_error("Error: binding socket.");
    }
	if (listen(this->listenSockets[i].sockfd, 32) < 0)
    {
		throw std::runtime_error("Error: listening socket.");
        close(this->listenSockets[i].sockfd);
    }
}

void HttpServer::createListen()
{
   /* struct sockaddr_in address;
    socklen_t addrlen;*/
	int	c;
    t_serv serv;

    c = 0;
    for (size_t i = 0; i < this->vec.size(); ++i)
    {
		std::map<std::string, std::string>::const_iterator it = vec[i].getListen().begin();
		while (it != vec[i].getListen().end())
        {
            serv.host = it->first;
            serv.port = atoi(it->second.c_str());
            this->listenSockets.push_back(serv);
            createSockets(c);
            c++;
            ++it;
        }
    }
}

void HttpServer::createacceptfd(int i, fd_set *initrset, int *maxfd)
{
	struct sockaddr_in	address;
	int					fd;
	socklen_t			addrlen;

	if ((fd = accept(this->listenSockets[i].sockfd, (struct sockaddr *)&address, \
					(socklen_t*)&addrlen)) < 0)
		return ;
	fcntl(fd, F_SETFL, O_NONBLOCK);
	FD_SET(fd, initrset);
	(*maxfd)++;
	this->acceptfds.insert(std::make_pair(fd, Client()));
}

int HttpServer::getrequest(int fd)
{
    char	buffer[BUFREAD];
    int		n = 0;

	n = recv(fd, buffer, sizeof(buffer) - 1, 0);
	buffer[n] = '\0';
	this->acceptfds[fd].setRequest(std::string(buffer));
//    std::cout << buffer << std::endl;
    return (n);
}

int HttpServer::sendresponse(int fd)
{
    std::string str;

    str = this->acceptfds[fd].getResponse(BUFWRITE);
    return (send(fd, str.c_str(), str.length(), 0));
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
        select(maxfd + 1, &readset, &writeset, 0, 0);
        for (size_t i = 0; i < this->listenSockets.size(); i++)
        {
            if (FD_ISSET(this->listenSockets[i].sockfd, &readset))
                createacceptfd(i, &initrset,  &maxfd);
        }
        std::map<int, Client>::iterator it = this->acceptfds.begin();
        while (it != this->acceptfds.end())
        {
            if (FD_ISSET(it->first, &readset))
            {
                getrequest(it->first);
                FD_SET(it->first, &initwset);
            }
            if (FD_ISSET(it->first, &writeset))
            {
               sendresponse(it->first);
               FD_CLR(it->first, &initwset);
               //close(it->first);
            }
            it++;
        }
    }
}
