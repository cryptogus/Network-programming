#include <iostream>
#include "tcp-server.h"

void usage()
{
    printf("syntax: server-test <port>\n");
    printf("sample: server-test 12341\n");
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        usage();
        return 0;
    }
    TcpServer server(argv[1]);
    if (server.Setaddr())
        std::cout << "server setting complete\n";
    else
        return 1;
    if (server.run() == -1) {
        return 2;
    }
    return 0;
}