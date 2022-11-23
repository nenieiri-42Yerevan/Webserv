#include "Cgi.hpp"
#include <fcntl.h>
#include "Client.hpp"

Cgi::Cgi(Client  *other)
{
    this->header = other->getHeader();
    this->cont = other;
    /*std::map<std::string, std::string>::iterator it = this->header.begin();
    while (it != this->header.end())
    {
        std::cout << it->first << "   " << it->second << std::endl;
        it++;
    }*/
}

Cgi::Cgi()
{
}

const std::string Cgi::findscript(std::string uri)
{
    size_t found;
    std::string ret;

    found = uri.find("?");
    if (found != std::string::npos)
        ret = uri.substr(0, found);
    else
        ret = uri;    
    return ret;
}

std::string Cgi::findquery(std::string uri)
{
    size_t found;

    found = uri.find("?");
    if (found != std::string::npos)
        return (uri.substr(found + 1, uri.length() - found));
    return ("");
}

void Cgi::initenv()
{
    env["AUTH_TYPE"] = this->header["method"];
    if (this->header["content-length"] != "")
        env["CONTENT_LENGTH"] = this->header["content-length"];
    env["GATEWAY_INTERFACE"] = "CGI/1.1";
    env["CONTENT_TYPE"] = "text/html";
    env["PATH_INFO"] = "";
    env["PATH_TRANSLATED"] = "";
    env["REQUEST_METHOD"] = "GET";
    env["QUERY_STRING"] = findquery(this->header["uri"]);
    env["REMOTE_ADDR"] = "127.0.0.1";
    env["SCRIPT_NAME"] = findscript(this->header["uri"]);
    env["SERVER_NAME"] = "127.0.0.1";
    env["SERVER_PORT"] = "8000";
    env["SERVER_PROTOCOL"] = "HTTP/1.1";
    env["SERVER_SOFTWARE"] = "Webserv";
}

Cgi::Cgi(const Cgi &other)
{
    *this = other;
}

void Cgi::tofile(std::string path)
{
    std::stringstream ss;
    std::ifstream ifs(path, std::ifstream::in);
    std::string response;
    std::string str;

    ss << ifs.rdbuf();
    str = ss.str();
    ss.clear();
    ss.str("");
    ss << str.length();
	response += "HTTP/1.1 " + (std::string)"200" + " " + "ok" + "\r\n";
	response += "Content-Type : text/html;\r\n";
	response += "Content-Length : " + ss.str() + "\r\n";
	response += "Server : webserv\r\n";
	response += "\r\n";
	response += str;
    this->cont->setResponse(response);
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
    char *envc[20];
    int i, status;
    int tmpfd;

    i = 0;
    initenv();
    tmpfd = dup(1);
    std::map<std::string, std::string>::iterator it = env.begin();
    while (it != env.end())
    {
        envc[i++] = strdup((it->first + "=" + it->second).c_str());
        it++;
    }
    envc[i] = NULL;
    int fd = open("temp", O_WRONLY | O_TRUNC | O_CREAT, S_IRWXU);
    pid = fork();
    if (pid == 0)
    {
        dup2(fd, 1);
        close(fd);
        args[0] = strdup("/usr/bin/php");
        args[1] = strdup(this->cont->getFile().c_str());
        args[2] = NULL;
        if (execve(args[0], args, envc) == -1)
            perror("Error\n");
    }
    if (pid < 0)
    {
        perror("Error: ");
    }
    waitpid(pid, &status, 0);
    dup2(tmpfd, 1);
    close(fd);
    tofile("temp");
    close(tmpfd);
}
