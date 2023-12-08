#pragma once

#include <iostream>
#include <stdint.h>
#include <cstring>

#ifdef __linux__
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#endif

class TcpClient {

private:
    //ipv4
    sockaddr_in clientaddr;
    //ipv6
    sockaddr_in6 clientaddr6;

public:
    TcpClient(void);
};