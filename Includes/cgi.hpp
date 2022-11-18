#ifndef CGI_HPP
# define CGI_HPP

#include "webserv.hpp"

class Cgi
{
    private:
        std::map<std::string, std::string> env;
        void initenv();
        void cgi_run();
    public:
        Cgi();
        Cgi(const Cgi &other);
        Cgi &operator=(Cgi const &other);
        virtual ~Cgi();

};

#endif