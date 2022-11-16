#include "cgi.hpp"

Cgi::Cgi()
{

}

void Cgi::initenv()
{
    env["AUTH_TYPE"] = "";
    env["CONTENT_LENGTH"] = "";
    env["GATEWAY_INTERFACE"] = "CGI/1.1";
}

Cgi::Cgi(const Cgi &other)
{
    *this = other;
}

Cgi::~Cgi(){}

Cgi &Cgi::operator=(Cgi const &other)
{
    if (this != &other)
    {
        this->env = other.env;
    }
    return (*this);
}