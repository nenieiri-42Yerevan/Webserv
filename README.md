# Webserv

Webserv is a HTTP/1.1 server written in C++98.

## Description

The objective of this project is to create a functional HTTP web server.
It conditional compliant with RFC.
It implements Get, Post and Delete methods. So it can serve static web pages, dynamic pages thanks to CGI (PHP, python).
It is fully configurable though a config file, inspired by Nginx format.

## Usage

```shell
# Compile the sources
make all
```
```shell
# Run the server
./webserv [config_file]
```

### Configuration File
See example of configuration in 'Configs' directory

## Features
### ✅ It supports:
* [GET, POST, DELETE] HTTP methods
* A configuration file given as an argument
* Php CGI
* Serve a fully static website
* Default error pages if none are provided
* File uploading
* Accurate HTTP response status codes
* Understand chunked requests
* Listen to multiple ports
* Run multiple sites

As bonus
* Support cookies and session management
* Python CGI

### ❌ It doesn't support:
* other HTTP methods

## Useful links
[__1) bind__](https://www.youtube.com/watch?v=059EKGJWilU) <br>
[__2) select, poll, epoll__](https://www.youtube.com/watch?v=CKgvjrDqriY) <br>
[__3) cookies, sessions__](https://www.youtube.com/watch?v=cpFfzE9eGT0)

## Authors

* [arastepa](https://github.com/arastepa)
* [vismaily](https://github.com/nenieiri)
