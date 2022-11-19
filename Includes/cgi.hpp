#ifndef CGI_HPP
# define CGI_HPP

#include "webserv.hpp"
#include <sys/types.h>
#include <sys/wait.h>

class Cgi
{
    private:
        std::map<std::string, std::string> env;
    public:
        Cgi();
        void initenv();
        void cgi_run();
        Cgi(const Cgi &other);
        Cgi &operator=(Cgi const &other);
        virtual ~Cgi();

};

#endif