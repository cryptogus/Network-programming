#include <iostream>
#include "tcp-server.h"

void usage(char *name)
{
    printf("syntax: %s <port>\n", name);
    printf("sample: %s 12341\n", name);
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        usage(argv[0]);
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