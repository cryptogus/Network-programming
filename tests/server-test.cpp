#include <iostream>
#include "server.h"

void usage()
{
    printf("syntax: server-test <ip version> <protocol>\n");
    printf("sample: server-test ipv4 TCP\n");
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        usage();
        return 0;
    }
    TcpServer server(argv[1], argv[2]);
    server.run();
}