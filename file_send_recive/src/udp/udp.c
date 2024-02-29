#include "udp.h"

/***
 * Bind UDP port
 */

static int udp_bind(struct sockaddr *addr, socklen_t* addrlen, const char *ip, const char *port) {
    struct addrinfo hints;
    struct addrinfo *result;
    int sock, flags;

    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_protocol = IPPROTO_UDP;

#ifdef UDPSERVER
    const char *host = "0.0.0.0";
#else
    const char *host = ip;
#endif
    if (0 != getaddrinfo(host, NULL, &hints, &result)) {
      perror("getaddrinfo error");
      exit(1);
    }

    if (result->ai_family == AF_INET)
      ((struct sockaddr_in *)result->ai_addr)->sin_port = htons(atoi(port));
    else if (result->ai_family == AF_INET6)
      ((struct sockaddr_in6 *)result->ai_addr)->sin6_port = htons(atoi(port));
    else {
      fprintf(stderr, "unknown ai_family %d", result->ai_family);
      freeaddrinfo(result);
      exit(1);
    }
    memcpy(addr, result->ai_addr, result->ai_addrlen);
    *addrlen = result->ai_addrlen;

  if (-1 == (sock = socket(result->ai_family, SOCK_DGRAM, IPPROTO_UDP))) {
    perror("Cannot create socket");
    freeaddrinfo(result);
    exit(1);
  }

#ifdef UDPSERVER
  if (0 != bind(sock, result->ai_addr, result->ai_addrlen)) {
    perror("Cannot bind");
    close(sock);
    freeaddrinfo(result);
    exit(1);
  }
#endif

 return sock;
}

// 주어진 파일 경로에서 파일 이름만 추출하여 반환하는 함수
static char* extract_name(const char* filename) {
    // 주어진 파일 경로에서 마지막 슬래시('/')를 찾음
    const char* last_slash = strrchr(filename, '/');

    // 찾은 슬래시의 다음 위치부터의 문자열이 파일 이름을 나타냄
    return (char*)(last_slash ? last_slash + 1 : filename);
}

void udp_client(const char *ip, const char *port, const char *filename) {

    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("Error opening file");
    }

    struct sockaddr_storage client_addr;
    socklen_t client_addrlen = sizeof(client_addr);

    int udp_sock = udp_bind((struct sockaddr *)&client_addr, &client_addrlen, ip, port);

    char buffer[BUFSIZ];
    size_t bytesRead;

    const char* name = extract_name(filename);
    char len = (char)strlen(name);
    sendto(udp_sock, &len, 1, 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
    sendto(udp_sock, name, strlen(name) + 1, 0, (struct sockaddr *)&client_addr, sizeof(client_addr));
    size_t tmp = BUFSIZ;
    while((bytesRead = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
          if (bytesRead < tmp)
            sleep(1); // test에서 너무 빨리 보내면 마지막 데이터를 서버가 recv를 못 하는 것을 발견했다.
         if (sendto(udp_sock, buffer, bytesRead, 0, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
            perror("Error sending data");
          }
    }

    fclose(fp);
    close(udp_sock);

    printf("File sent successfully.\n");
}

void udp_server(const char *port) {

    struct sockaddr_storage client_addr;
    socklen_t client_addrlen = sizeof(client_addr);

    int udp_sock = udp_bind((struct sockaddr *)&client_addr, &client_addrlen, NULL, port);
    char buffer[BUFSIZ];
    socklen_t client_len = sizeof(client_addr);

    // filename length
    recvfrom(udp_sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_len);
    // filename 변수 크기가 크면 남는 공간에 이후에 올 send 데이터를 저장해버려서 파일 write시 데이터 손실 생김
    char filename[buffer[0] + 1];
    recvfrom(udp_sock, filename, sizeof(filename), 0, (struct sockaddr *)&client_addr, &client_len);

    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("Error opening file");
    }

     while (1) {

        ssize_t reval = recvfrom(udp_sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_len);
        if (reval < 0) {
            perror("Error receiving data");
            exit(1);
        }

        fwrite(buffer, 1, reval, fp);
        //printf("%d\n",reval);

        if (reval < sizeof(buffer))
            break;
    }

    fclose(fp);
    close(udp_sock);

    printf("File recieve successfully.\n");
}