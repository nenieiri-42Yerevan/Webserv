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
    if (this->listenSockets[i].host == "*")
    {
        this->listenSockets[i].host = "0.0.0.0";
    }
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
	std::cout << "Server: " << this->listenSockets[i].host << ":";
	std::cout << this->listenSockets[i].port << " has created on fd: ";
	std::cout << this->listenSockets[i].sockfd << "." << std::endl;
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
            serv.current = i;
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
	if (fd > *maxfd)
        *maxfd = fd;
	this->acceptfds.insert(std::make_pair(fd, Client(this->vec,  this->listenSockets[i].current)));
}

int HttpServer::getrequest(int fd)
{
    char	buffer[BUFREAD];
    int		n = 0;

	n = recv(fd, buffer, sizeof(buffer) - 1, 0);
    if (n <= 0)
        return (n);
	buffer[n] = '\0';
	this->acceptfds.at(fd).setRequest(std::string(buffer));
    return (n);
}

int HttpServer::sendresponse(int fd)
{
    std::string str;

    str = this->acceptfds.at(fd).getResponse(BUFWRITE);
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
                int ret = getrequest(it->first);
                if (ret == -1)
                    continue;
                else if (ret == 0)
                {
                    FD_CLR(it->first, &initwset);
                    FD_CLR(it->first, &initrset);
                    close(it->first);
                    this->acceptfds.erase(it++);
                    continue ;
                }
                FD_SET(it->first, &initwset);
            }
            else
            { 
                if (FD_ISSET(it->first, &writeset))
                {
                    int res = sendresponse(it->first);
                    if (res == -1)
                        continue ;
                    if (it->second.getSendStatus() == true)
                        FD_CLR(it->first, &initwset);
                }
                if (it->second.getCloseStatus() == true)
                {
                    FD_CLR(it->first, &initwset);
                    FD_CLR(it->first, &initrset);
                    close(it->first);
                    this->acceptfds.erase(it++);
                    continue ;
                }
            }
            it++;
        }
    }
}
