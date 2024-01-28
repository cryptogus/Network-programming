#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/ip.h>
#include <netinet/udp.h>

#define UDP_PORT 12345
#define BUFFER_SIZE 2048

void modify_udp_packet(char* buffer, ssize_t data_size) {
    // 이 함수에서는 UDP 패킷의 변조 작업을 수행
    // 여기서는 단순히 문자열 "Modified: "를 추가
    char modified_data[] = "Modified: ";
    size_t modified_data_size = strlen(modified_data);

    // 기존 데이터를 뒤로 옮기고 새로운 데이터를 추가
    memmove(buffer + modified_data_size, buffer, data_size);
    memcpy(buffer, modified_data, modified_data_size);
}

int main() {
    int raw_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Raw 소켓 생성
    if ((raw_socket = socket(AF_INET, SOCK_RAW, IPPROTO_UDP)) == -1) {
        perror("Raw socket creation failed");
        exit(EXIT_FAILURE);
    }

    // 서버 주소 설정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(UDP_PORT);

    // 소켓 바인딩
    if (bind(raw_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }

    while (1) {
        // UDP 패킷 수신
        ssize_t data_size = recvfrom(raw_socket, buffer, sizeof(buffer), 0, (struct sockaddr*)&client_addr, &addr_len);
        if (data_size == -1) {
            perror("Receive failed");
            exit(EXIT_FAILURE);
        }

        // 패킷 변조
        modify_udp_packet(buffer, data_size);

        // 변조된 패킷 송신
        ssize_t bytes_sent = sendto(raw_socket, buffer, data_size + strlen("Modified: "), 0, (struct sockaddr*)&client_addr, addr_len);
        if (bytes_sent == -1) {
            perror("Send failed");
            exit(EXIT_FAILURE);
        }

        printf("Received UDP packet, modified, and sent it back.\n");
    }

    // 소켓 닫기
    close(raw_socket);

    return 0;
}
