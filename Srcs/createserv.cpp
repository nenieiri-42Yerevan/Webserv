#include "../Includes/webserv.hpp"

void createserv()
{
    int sockfd;
    struct sockaddr_in address;
    int port;
    socklen_t addrlen;
    int acceptfd;

    port = 8080;
    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        throw std::runtime_error("fatal error");
    }
    memset((char *)&address, 0, sizeof(address));
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(2130706433);
    address.sin_port = htons(port);
    if ((bind(sockfd, (struct sockaddr *) &address, sizeof(address))) < 0)
    {
        throw std::runtime_error("error: bind");
    }
    if (listen(sockfd, 5) < 0)
    {
        throw std::runtime_error("error: listen");
    }
    addrlen = sizeof(address);
    if ((acceptfd = accept(sockfd,
		(struct sockaddr *)&address, (socklen_t*)&addrlen)) < 0)
    {
        throw std::runtime_error("Error: accept");
    }
}