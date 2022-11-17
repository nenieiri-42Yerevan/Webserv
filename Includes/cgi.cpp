#include "cgi.hpp"

Cgi::Cgi()
{

}

void Cgi::initenv()
{
    env["AUTH_TYPE"] = "";
    env["CONTENT_LENGTH"] = "";
    env["GATEWAY_INTERFACE"] = "CGI/1.1";
    env["CONTENT_TYPE"] = "text/html";
    env["PATH_INFO"] = "";
    env["PATH_TRANSLATED"] = "";
    env["REQUEST_METHOD"] = "GET";
    env["REMOTE_ADDR"] = "";
    env["SCRIPT_NAME"] = "/cgi/hello.php";
    env["SERVER_NAME"] = "127.0.0.1";
    env["SERVER_PORT"] = "8000";
    env["SERVER_PROTOCOL"] = "HTTP/1.1";
    env["SERVER_SOFTWARE"] = "Webserv";
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

void Cgi::cgi_run()
{
    int pid;
    char *args[3];
    char *envc[13];
    int i;

    i = 0;
    initenv();
    std::map<std::string, std::string>::iterator it = env.begin();
    while (it != env.end())
    {
        envc[i++] = strdup((it->first + "=" + it->second).c_str());
        it++;
    }
    envc[i] = NULL;
    int fd = open("tmp", O_WRONLY | O_TRUNC);
    pid = fork();
    if (pid == 0)
    {
        args[0] = "/usr/bin/php";
        args[1] = (char *)env["SCRIPT_NAME"].c_str();
        args[3] = NULL;
        dup2(fd, 0);
        close(fd);
        execve("/usr/bin/php", args, envc);
    }
    if (pid < 0)
    {
        perror("Error: ");
    }
    //waitpid[]


}