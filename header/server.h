#pragma once

#include <iostream>
#include <stdint.h>

#ifdef __linux__
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>

#endif
// ip ver 4 or 6
// TCP: SOCK_STREAM, UDP: SOCK_DGRAM

class TcpServer
{

private:
    // ipv4
    sockaddr_in addr;
    // ipv6
    sockaddr_in6 addr6;

    // bind

    // listen
public:
    // 생성자
    // loof back
    TcpServer(/*char *ip = "127.0.0.1"*/) {
        inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
        inet_pton(AF_INET6, "::1", &addr6.sin6_addr); //0:0:0:0:0:0:0:1
    }

    void PtonResult(void);
    void NtopResult(void);
    // check value
    bool Setaddr (sockaddr_in addr_) const;
    bool Setaddr6 (sockaddr_in6 addr6_) const;
};


/*
    __linux__       Defined on Linux
    __sun           Defined on Solaris
    __FreeBSD__     Defined on FreeBSD
    __NetBSD__      Defined on NetBSD
    __OpenBSD__     Defined on OpenBSD
    __APPLE__       Defined on Mac OS X
    __hpux          Defined on HP-UX
    __osf__         Defined on Tru64 UNIX (formerly DEC OSF1)
    __sgi           Defined on Irix
    _AIX            Defined on AIX
    _WIN32          Defined on Windows
*/