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
// ipv4만
int TcpClient::run(void) {
    sock_ = socket(addr.sin_family, SOCK_STREAM, 0);
    if (sock_ == -1) {
        fprintf(stderr, "Socket creation error\n");
        return -1;
    }
    int retval = 0;
    retval = connect(sock_, (struct sockaddr *)&addr, sizeof(addr));
    if (retval == -1) {
        fprintf(stderr, "Connection fail\n");
        return -1;
    }

    // 서버에 보낼 데이터
    while(true) {
        std::cout << "Enter data to send\n" << std::endl;
        std::string buf = "";
        std::getline(std::cin, buf);
        if (buf == "")
            break;
        /* char 입력 함수, buf는 멤버 변수
        if (fgets(buf, BUFSIZ + 1, stdin) == NULL)
            break;

        // '\n' 제거
        int len = 0;
        if (buf[len - 1] == '\n')
            buf[len - 1] = '\0';
        if (strlen(buf) == 0)
            break;
        */

        // 데이터 보내기, 두번째 인자로 문자열의 첫번째 주소를 넣어 주면 됨  -> buf.begin()?
        retval = send(sock_, buf.c_str(), buf.length(), 0);
        if (retval == -1) {
        fprintf(stderr, "Data sending fail\n");
        break; 
        }

        printf("[TCP client] %d-byte is sended\n", retval);

        // 데이터 받기
        retval = recv(sock_, this->buf, retval, MSG_WAITALL);
        if (retval == -1) {
           fprintf(stderr, "Data recving fail\n");
        break; 
        }else if (retval == 0)
            break;

        this->buf[retval] = '\0';
        printf("recving data is %s , %d-bytes\n", this->buf, retval);
    }

    close(sock_);


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