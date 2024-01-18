#pragma once

#include <iostream>
#include <cerrno>
#include <cstring>
#include <unistd.h>

/* Network headers */
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

class UDP_server {

private:
    addrinfo hints_;
    addrinfo *result_;
    
protected:
    int sock_;
    struct sockaddr_storage client_addr; // sockaddr_in clientaddr;
    socklen_t client_addrlen;
public:
    UDP_server(char *port);
    virtual int run(void);
};