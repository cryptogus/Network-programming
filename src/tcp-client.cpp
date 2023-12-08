#include "tcp-client.h"

TcpClient::TcpClient(void) {
    memset(&clientaddr, 0 ,sizeof(clientaddr));
    memset(&clientaddr6, 0 ,sizeof(clientaddr6));

    clientaddr.sin_family = AF_INET;
    clientaddr6.sin6_family = AF_INET6;

    inet_pton(AF_INET, "127.0.0.1", &clientaddr.sin_addr);
    inet_pton(AF_INET6, "::1", &clientaddr6.sin6_addr); //0:0:0:0:0:0:0:1

    // port는 12345로 고정한다.
    clientaddr.sin_port = htons(12345);
    clientaddr6.sin6_port = htons(12345);
    // this->ip=ip;
    inet_pton(AF_INET, ip, &clientaddr.sin_addr);
}