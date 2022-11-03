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
        this->clientSockets = other.clientSockets;
        this->listenSockets = other.listenSockets;
    }
    return (*this);
}

HttpServer::HttpServer(std::vector<Server> *vec)
{
    this->vec = *vec;
}

void HttpServer::createListen()
{
   /* struct sockaddr_in address;
    socklen_t addrlen;*/
    int c;

    c = 0;
    for (size_t i = 0; i < this->vec.size(); i++)
    {
        std::map<std::string, std::string>::const_iterator it = vec[i].getListen().begin();
        while (it != vec[i].getListen().end())
        {
            this->listenSockets[c].port = atoi(it->second.c_str());
            this->listenSockets[c].host = it->first;
            c++;
            it++;
        }
    }


}