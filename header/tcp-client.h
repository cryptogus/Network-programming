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
    sockaddr_in addr;
    //ipv6
    sockaddr_in6 addr6;

public:
    TcpClient(void);
    TcpClient(char *ip, char *port);

    void PtonResult(void);
    void NtopResult(void);
    // check value
    bool Setaddr (void) const;
    bool Setaddr6 (void) const;
};