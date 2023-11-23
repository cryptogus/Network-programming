#include <iostream>
#include "server.h"
#include <cstring>

void usage()
{
    printf("syntax: server-test <ip version> <protocol>\n");
    printf("sample: server-test 4 TCP\n");
}

int main(int argc, char *argv[])
{
    net_protocol pro;

    if (argv[1] == '4')
        pro.domain = AF_INET;
    else if (argv[1] == '6')
        pro.domain = AF_INET6;
    else if (strcmp(argv[1], "bluetooth")) {
        pro.domain = AF_BTH;
    }
    else {
        fprintf(stderr,"domain error\n");
        return -1;
    }

    if (strcmp(argv[2], "TCP") == 0)
    {
        pro.type = SOCK_STREAM;
    }else if (strcmp(argv[2], "UDP") == 0)
    {
        pro.type = SOCK_DGRAM;
    }
    else
    {
        fprintf(stderr,"type error\n");
        return -2;
    }

    int sock = socket(pro.domain, pro.domain, 0);

    close(sock);
}