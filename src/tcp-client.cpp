#include "tcp-client.h"

TcpClient::TcpClient(void) {
    memset(&addr, 0 ,sizeof(addr));
    memset(&addr6, 0 ,sizeof(addr6));

    addr.sin_family = AF_INET;
    addr6.sin6_family = AF_INET6;

    inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    inet_pton(AF_INET6, "::1", &addr6.sin6_addr); //0:0:0:0:0:0:0:1

    // port는 12345로 고정한다.
    addr.sin_port = htons(12345);
    addr6.sin6_port = htons(12345);
    
}

TcpClient::TcpClient(char *ip, char *port) {
    memset(&addr, 0 ,sizeof(addr));
    
    addr.sin_family = AF_INET;
    // this->ip=ip;
    inet_pton(AF_INET, ip, &addr.sin_addr);
    int port_ = std::stoi(port);
    addr.sin_port = htons(port_);
}

void TcpClient::PtonResult(void) {
	printf("(IP) presentation to numberic\n");
    printf("%#x\n", addr.sin_addr.s_addr);
    //in6_addr ip;
    //ip.s6_addr == ip.__in6_u.__u6_addr8;
    for (int i = 0; i < 16; i++)
        printf("%02x", addr6.sin6_addr.__in6_u.__u6_addr8[i]);
    printf("\n");
}

void TcpClient::NtopResult(void) {
    printf("(IP) numberic to presentation\n");
    char ip4Buffer[INET_ADDRSTRLEN];
    char ip6Buffer[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr.sin_addr, ip4Buffer, sizeof(ip4Buffer));
    inet_ntop(AF_INET6, &addr6.sin6_addr, ip6Buffer, sizeof(ip6Buffer));
    printf("%s\n", ip4Buffer);
    printf("%s\n", ip6Buffer);
}

// only ipv4
bool TcpClient::Setaddr(void) const {
    if (addr.sin_family != AF_INET) {
        fprintf(stderr, "ipv4 family is error\n");
        return false;
    }
    if (!addr.sin_addr.s_addr) {
        fprintf(stderr, "please input ip\n");
        return false;
    }
    if (!addr.sin_port) {
        fprintf(stderr, "please input port\n");
        return false;
    }
    return true;
}
// unfinished
bool TcpClient::Setaddr6(void) const {
    if (addr6.sin6_family != AF_INET6) {
        fprintf(stderr, "ipv6 family is error\n");
        return false;
    }
    if (!addr6.sin6_addr.s6_addr32[0] && !addr6.sin6_addr.s6_addr32[1] && !addr6.sin6_addr.s6_addr32[2] && !addr6.sin6_addr.s6_addr32[3]) {
        fprintf(stderr, "please input ipv6\n");
        return false;
    }
    if (!addr6.sin6_port) {
        fprintf(stderr, "please input port\n");
        return false;
    }
    return true;
}