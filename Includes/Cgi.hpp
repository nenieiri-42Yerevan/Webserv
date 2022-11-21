#ifndef CGI_HPP
# define CGI_HPP

#include <sys/types.h>
#include <sys/wait.h>
#include "Client.hpp"
class Client;

class Cgi
{
    private:
        std::map<std::string, std::string> env;
        Client c;
    public:
        Cgi();
        Cgi(const Client &other);
        void initenv();
        void cgi_run();
        Cgi(const Cgi &other);
        Cgi &operator=(Cgi const &other);
        virtual ~Cgi();

};

#endif