#include "tcp.h"
// https://www.educative.io/answers/how-to-implement-tcp-sockets-in-c
/***
 * Bind TCP port
 */

int tcp_bind(struct sockaddr *addr, socklen_t* addrlen, const char *ip, const char *port) {
    struct addrinfo hints;
    struct addrinfo *result;
    int sock, flags;

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;

#ifdef TCPSERVER
    const char *host = "0.0.0.0";
#else
    const char *host = ip;
#endif
    if (0 != getaddrinfo(host, NULL, &hints, &result)) {
      perror("getaddrinfo error");
    }

    if (result->ai_family == AF_INET)
      ((struct sockaddr_in *)result->ai_addr)->sin_port = htons(atoi(port));
    else if (result->ai_family == AF_INET6)
      ((struct sockaddr_in6 *)result->ai_addr)->sin6_port = htons(atoi(port));
    else {
      fprintf(stderr, "unknown ai_family %d", result->ai_family);
      freeaddrinfo(result);
      return -1;
    }
    memcpy(addr, result->ai_addr, result->ai_addrlen);
    *addrlen = result->ai_addrlen;

  if (-1 == (sock = socket(result->ai_family, SOCK_STREAM, IPPROTO_TCP))) {
    perror("Cannot create socket");
    freeaddrinfo(result);
  }

#ifdef TCPSERVER
  if (0 != bind(sock, result->ai_addr, result->ai_addrlen)) {
    perror("Cannot bind");
    close(sock);
    freeaddrinfo(result);
  }
#endif

 return sock;
}

int tcp_client(const char *ip, const char *port, const char *filepath) {

    FILE *fp = fopen(filepath, "rb");
    if (!fp) {
        perror("Error opening file");
    }

    struct sockaddr_storage client_addr;
    socklen_t client_addrlen = sizeof(client_addr);

    int tcp_sock = tcp_bind((struct sockaddr *)&client_addr, &client_addrlen, ip, port);
    if (connect(tcp_sock, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0)
        perror("connect error");

    char buffer[BUFSIZ];
    size_t bytesRead;

    while((bytesRead = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        printf("%ld\n", bytesRead);
         if (send(tcp_sock, buffer, bytesRead, 0) < 0) {
            perror("Error sending data");
        }
        sleep(2); // 정보를 받고 server에서는 client로 ack를 보내는데 지금 이 속도가 너무 빨라서 서버에서 데이터를 다 못 받아오고 client만 종료됨, wireshark 로 패킷 캡처해보길
    }

    fclose(fp);
    close(tcp_sock);

    printf("File sent successfully.\n");

    return 0;
}

int tcp_server(const char *port, const char *filepath) {

    FILE *fp = fopen(filepath, "wb");
    if (!fp) {
        perror("Error opening file");
    }

    struct sockaddr_storage client_addr;
    socklen_t client_addrlen = sizeof(client_addr);

    int tcp_sock = tcp_bind((struct sockaddr *)&client_addr, &client_addrlen, NULL, port);

    if (listen(tcp_sock, 5) < 0)
        perror("listen fail");

    int client_sock = accept(tcp_sock, (struct sockaddr *)&client_addr, &client_addrlen);
    if (client_sock < 0)
        perror("accept fail");

    char buffer[BUFSIZ];

     while (1) {
        socklen_t client_len = sizeof(client_addr);
        ssize_t reval = recv(client_sock, buffer, sizeof(buffer), 0);
        printf("%ld\n", reval);
        if (reval < 0) {
            perror("Error receiving data");
        } else if (reval == 0) {
            break;  // Transmission complete, TCP의 recv 함수가 0을 반환하는 경우는 일반적으로 송신자가 연결을 닫은 경우입니다.
            // 이것은 TCP의 한 특징으로, 소켓 연결이 양방향 통신에서 닫히면 recv 함수가 0을 반환합니다.
            // 이것은 EOF(End of File) 조건을 나타내며, 클라이언트가 더 이상 데이터를 전송하지 않고 연결을 종료했음을 의미합니다.
        }
        fwrite(buffer, 1, reval, fp);
    }

    fclose(fp);
    close(client_sock);
    close(tcp_sock);
}