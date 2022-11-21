#ifndef CGI_HPP
# define CGI_HPP

#include <sys/types.h>
#include <sys/wait.h>
#include "Client.hpp"

class Cgi
{
    private:
        std::map<std::string, std::string> env;
        std::map<std::string, std::string> header;
        Client c;
    public:
        Cgi();
        Cgi(Client &other);
        void initenv();
        void cgi_run();
        Cgi(const Cgi &other);
        Cgi &operator=(Cgi const &other);
        void tofile(std::string path);
        virtual ~Cgi();

};

#endif