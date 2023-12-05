#include "server.h"

void TcpServer::PtonResult(void) {
	printf("(IP) presentation to numberic\n");
    printf("%#x\n", addr.sin_addr.s_addr);
    //in6_addr ip;
    //ip.s6_addr == ip.__in6_u.__u6_addr8;
    for (int i = 0; i < 16; i++)
        printf("%02x", addr6.sin6_addr.__in6_u.__u6_addr8[i]);
    printf("\n");
}

void TcpServer::NtopResult(void) {
    printf("(IP) numberic to presentation\n");
    char ip4Buffer[INET_ADDRSTRLEN];
    char ip6Buffer[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr.sin_addr, ip4Buffer, sizeof(ip4Buffer));
    inet_ntop(AF_INET6, &addr6.sin6_addr, ip6Buffer, sizeof(ip6Buffer));
    printf("%s\n", ip4Buffer);
    printf("%s\n", ip6Buffer);
}
	

