#include "tcp-server.h"

TcpServer::TcpServer(void) {
    memset(&addr, 0 ,sizeof(addr));
    memset(&addr6, 0 ,sizeof(addr6));

    addr.sin_family = AF_INET;
    addr6.sin6_family = AF_INET6;
    // for client 향후 client로 이동 예정
	inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
    inet_pton(AF_INET6, "::1", &addr6.sin6_addr); //0:0:0:0:0:0:0:1
    // server는 ip주소를 줄 필요가 없다. -> 위의 memset으로 해결했지만 제대로 명시하기 위해 남겨둠 
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr6.sin6_addr.__in6_u.__u6_addr32[0] = htonl(INADDR_ANY);
    addr6.sin6_addr.__in6_u.__u6_addr32[1] = htonl(INADDR_ANY);
    addr6.sin6_addr.__in6_u.__u6_addr32[2] = htonl(INADDR_ANY);
    addr6.sin6_addr.__in6_u.__u6_addr32[3] = htonl(INADDR_ANY);
    // port는 12345로 고정한다.
    addr.sin_port = htons(12345);
    addr6.sin6_port = htons(12345);
}

TcpServer::TcpServer(char *ip, char *port)/* : ipv4(ip)*/{
    memset(&addr, 0 ,sizeof(addr));
    
    addr.sin_family = AF_INET;

    inet_pton(AF_INET, ip, &addr.sin_addr);
    int port_ = std::stoi(port);
    addr.sin_port = htons(port_);
}

TcpServer::~TcpServer(void) {
    close(listen_sock_);
}

int TcpServer::run(void) {
    listen_sock_ = socket(addr.sin_family, SOCK_STREAM, 0);
    if (listen_sock_ == -1) {
        fprintf(stderr, "Socket creation error\n");
        return -1;
    }
/*
bind() - 주소정보 할당

○ 함수원형
  int bind(int sockfd, struct sockaddr *myaddr, int addrlen)

○ 리턴 값
  성공 시 : 0, 실패 시 : -1

- sockfd : 주소를 할당하고자 하는 소켓의 파일 디스크립터
- myaddr : 할당하고자 하는 주소 정보를 지니고 있는 sockaddr 구조체 변수의 포인터 인자 값
- addrlen : 인자로 전달된 주소 정보 구조체의 길이

=> 함수 호출이 성공하면 sockfd가 가리키는 소켓에 myaddr이 가리키는 주소 정보가 할당된다.

*/
    if (bind(listen_sock_, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        fprintf(stderr, "Socket bind error\n");
        return -1;
    }
/*
listen 함수호출을 통해서 "연결 요청 대기상태"
*/
    if (listen(listen_sock_, SOMAXCONN) == -1) {
        fprintf(stderr, "Socket bind error\n");
        return -1;
    }
/*
accept: 클라이언트 접속 요청 수락 함수
*/
    while(true) {
        socklen_t addrlen = 0;
        client_sock_ = accept(listen_sock_, (struct sockaddr *)&client_addr, &addrlen);
        if (client_sock_ == -1) {
            fprintf(stderr, "accept error\n");
            return -1;
        }

        // 접속한 client 정보
        inet_ntop(AF_INET, &client_addr.sin_addr, ipv4, sizeof(ipv4));
        printf("\n[TCP 서버] client 정보: IP: %s, Port:%d\n", ipv4, ntohs(client_addr.sin_port));

        while (true) {
            int retval = recv(client_sock_, buf, BUFSIZ, 0); 
            if (retval == -1) {
                fprintf(stderr, "recv fail\n");
                break;
            }else if (retval == 0) {
                // 입력이 아무것도 없을 때
                break;
            }

            // 받은 데이터 출력
            buf[retval + 1] = '\0';
            printf("\n[TCP %s:%d] %s\n",ipv4, ntohs(client_addr.sin_port), buf);

            // send -> 데이터 보내기, client가 보낸만큼 다시 보내 줄 것
            retval = send(client_sock_, buf, retval, 0);
            if (retval == -1) {
                fprintf(stderr, "send error\n");
                break;
            }
        }
        close(client_sock_);
    }
    close(listen_sock_);

}

// for client 향후 client로 이동 예정
void TcpServer::PtonResult(void) {
	printf("(IP) presentation to numberic\n");
    printf("%#x\n", addr.sin_addr.s_addr);
    //in6_addr ip;
    //ip.s6_addr == ip.__in6_u.__u6_addr8;
    for (int i = 0; i < 16; i++)
        printf("%02x", addr6.sin6_addr.__in6_u.__u6_addr8[i]);
    printf("\n");
}
// for client 향후 client로 이동 예정
void TcpServer::NtopResult(void) {
    printf("(IP) numberic to presentation\n");
    char ip4Buffer[INET_ADDRSTRLEN];
    char ip6Buffer[INET6_ADDRSTRLEN];
    inet_ntop(AF_INET, &addr.sin_addr, ip4Buffer, sizeof(ip4Buffer));
    inet_ntop(AF_INET6, &addr6.sin6_addr, ip6Buffer, sizeof(ip6Buffer));
    printf("%s\n", ip4Buffer);
    printf("%s\n", ip6Buffer);
}

// only ipv4
bool TcpServer::Setaddr(void) const {
    if (addr.sin_family != AF_INET) {
        fprintf(stderr, "ipv4 family is error\n");
        return false;
    }
    // for client 향후 client로 이동 예정
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
bool TcpServer::Setaddr6(void) const {
    if (addr6.sin6_family != AF_INET6) {
        fprintf(stderr, "ipv6 family is error\n");
        return false;
    }
    // for client 향후 client로 이동 예정
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