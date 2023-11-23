#include <iostream>
#include "server.h"
#include <cstring>

void usage()
{
    printf("syntax: server-test <ip version> <protocol>\n");
    printf("sample: server-test ipv4 TCP\n");
}

int main(int argc, char *argv[])
{
    if (argc != 3){
        usage();
        return 0;
    }
    net_protocol pro;

    if (strcmp(argv[1], "ipv4"))
        pro.domain_ = AF_INET;
    else if (strcmp(argv[1], "ipv6"))
        pro.domain_ = AF_INET6;
    /*else if (strcmp(argv[1], "bluetooth")) {
        pro.domain = AF_BTH;
    }*/
    else {
        fprintf(stderr,"domain error\n\n");
        usage();
        return -1;
    }

    if (strcmp(argv[2], "TCP") == 0)
    {
        pro.type_ = SOCK_STREAM;
    }else if (strcmp(argv[2], "UDP") == 0)
    {
        pro.type_ = SOCK_DGRAM;
    }
    else
    {
        fprintf(stderr,"type error\n\n");
        usage();
        return -2;
    }

    int sock = socket(pro.domain_, pro.type_, 0);
    if (sock == -1)
    {
        fprintf(stderr, "socket creating is error\n");
        return -3;
    }
    
    std::cout<< "socket creating is success\n";
    close(sock);
}