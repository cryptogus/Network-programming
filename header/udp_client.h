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

class UDP_client {

private:
    addrinfo hints_;
    addrinfo *result_;

    char buf[BUFSIZ + 1];
protected:
    int sock_;
    struct sockaddr_storage server_addr;
    socklen_t server_addrlen;
public:
    UDP_client(char *ip ,char *port);
    virtual int run(void);
    // ~UDP_client() {
    //     freeaddrinfo(result_);
    //     std::cout << "freeaddrinfo(result_)\n";
    // }
};