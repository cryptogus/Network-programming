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
// ip ver 4 or 6
// TCP: SOCK_STREAM, UDP: SOCK_DGRAM

class TcpServer
{

private:
    // ipv4
    sockaddr_in addr;
    char ipv4[INET_ADDRSTRLEN];
    // ipv6
    sockaddr_in6 addr6;
    char ipv6[INET6_ADDRSTRLEN];
    // socket
    int listen_sock_;
public:
    // 생성자(루프백으로 초기화, port 12345)
    TcpServer();
    // ipv4
    // TcpServer(char *ip = "127.0.0.1", char *port = "12345");
    TcpServer(char *ip, char *port);
    ~TcpServer();
    
    // server 실행
    int run(void);

    void PtonResult(void);
    void NtopResult(void);
    // check value
    bool Setaddr (void) const;
    bool Setaddr6 (void) const;
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