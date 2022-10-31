#include "../Includes/webserv.hpp"

void createserv()
{
    t_serv data;
    struct sockaddr_in address;
    socklen_t addrlen;

    std::string reply = "HTTP/1.1 200 OK\n\
Server: Hello\n\
Content-Length: 13\n\
Content-Type: text/plain\n\n\
Hello, world";
    data.port = 8080;
    if ((data.sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        throw std::runtime_error("fatal error");
    }
    memset((char *)&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(2130706433);
    address.sin_port = htons(data.port);
    if ((bind(data.sockfd, (struct sockaddr *) &address, sizeof(address))) < 0)
    {
        throw std::runtime_error("error: bind");
    }
    if (listen(data.sockfd, 5) < 0)
    {
        throw std::runtime_error("error: listen");
    }
    addrlen = sizeof(address);
    data.maxfd = data.sockfd;
    FD_ZERO(&data.actual_set);
    FD_SET(data.sockfd, &data.actual_set);
    while (1)
    {
        data.read_set = data.actual_set;
        data.write_set = data.actual_set;
        if (select(data.maxfd + 1, &data.read_set, &data.write_set, NULL, NULL) < 0)
            continue;
        if (FD_ISSET(data.sockfd, &data.read_set))
        {
            if ((data.acceptfd = accept(data.sockfd,
                (struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
            {
                throw std::runtime_error("Error: accept");
            }
            FD_SET(data.acceptfd, &data.actual_set);
            send(data.acceptfd, reply.c_str(), strlen(reply.c_str()), 0);
            break;
        }
    }
}