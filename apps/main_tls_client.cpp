#include <iostream>
#include "tls_client.h"

void usage(char *name)
{
    printf("syntax: %s <ipv4> <port>\n", name);
    printf("sample: %s 10.1.1.3 12341\n", name);
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        usage(argv[0]);
        return 0;
    }
    
    TlsClient Tlsclient(argv[1], argv[2]);

    if (Tlsclient.Setaddr())
        std::cout << "client setting complete\n\n";
    else
        return 1;
    if (Tlsclient.run() == -1) {
        return 2;
    }
}