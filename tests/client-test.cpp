#include <iostream>
#include "tcp-client.h"

int main(void) {

    TcpClient client;
    printf("Test n to p\n");
    client.NtopResult();
    printf("Test p to n\n");
    client.PtonResult();

    if (client.Setaddr())
        std::cout << "client setting complete\n";
    else
        return 1;
    if (client.run() == -1) {
        return 2;
    }
    return 0;
}