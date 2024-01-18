#include <iostream>
#include "tcp-server.h"

int main(void)
{
    TcpServer server;
    if (server.Setaddr())
        std::cout << "server setting complete\n";
    else
        return 1;
    if (server.run() == -1) {
        return 2;
    }
    return 0;

}