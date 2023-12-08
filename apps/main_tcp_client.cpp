#include <iostream>
#include "tcp-client.h"

void usage()
{
    printf("syntax: client-test <ipv4> <port>\n");
    printf("sample: client-test 10.1.1.3 12341\n");
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        usage();
        return 0;
    }
    
    TcpClient client(argv[1], argv[2]);

    printf("=====Test n to p=====\n");
    client.NtopResult();
    printf("\n=====Test p to n=====\n");
    client.PtonResult();
    printf("\n");
    if (client.Setaddr())
        std::cout << "client setting complete\n\n";
    else
        return 1;
    if (client.run() == -1) {
        return 2;
    }
}