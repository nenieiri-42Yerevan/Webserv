#ifndef HTTPSERVER_HPP
#define HTTPSERVER_HPP

#include <vector>
#include "webserv.hpp"
#include <map>

class HttpServer
{
    public:
        HttpServer();
        HttpServer(std::vector<Server> *vec);
        virtual ~HttpServer();
        HttpServer(const HttpServer &other);
        HttpServer &operator=(const HttpServer &other);
        void createListen();
        void createSockets(int i);
    private:
        std::vector<t_serv> clientSockets;
        std::vector<t_serv> listenSockets;
        std::vector<Server> vec;

};

#endif