#include "tcp.h"

/***
 * Bind TCP port
 */

static int tcp_bind(struct sockaddr *addr, socklen_t* addrlen, const char *ip, const char *port) {
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

  if (-1 == (sock = socket(result->ai_family, SOCK_STREAM, IPPROTO_TCP))) {
    perror("Cannot create socket");
    freeaddrinfo(result);
    exit(1);
  }

#ifdef TCPSERVER
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


void tcp_client(const char *ip, const char *port, const char *filename) {

    FILE *fp = fopen(filename, "rb");
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

    const char* name = extract_name(filename);
    char len = (char)strlen(name);
    send(tcp_sock, &len, 1, 0);
    sleep(1);
    send(tcp_sock, name, strlen(name) + 1, 0);

    while((bytesRead = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        if (send(tcp_sock, buffer, bytesRead, 0) < 0) {
            perror("Error sending data");
        }
        sleep(1);
    }

    fclose(fp);
    close(tcp_sock);

    printf("File sent successfully.\n");
}

void *handle_client(void *arg) {
    struct ThreadArgs *args = (struct ThreadArgs *)arg;

    char buffer[BUFSIZ];
    // filename length
    recv(args->client_sock, buffer, sizeof(buffer), 0);
    // filename 변수 크기가 크면 남는 공간에 이후에 올 send 데이터를 저장해버려서 파일 write시 데이터 손실 생김
    char filename[buffer[0] + 1];
    recv(args->client_sock, filename, sizeof(filename), 0);

    FILE *fp = fopen(filename, "wb");
    if (!fp) {
        perror("Error opening file");
    }
    args->fp = fp;

    while (1) {
        memset(buffer, 0, sizeof(buffer));
        ssize_t reval = recv(args->client_sock, buffer, sizeof(buffer), 0);

        if (reval < 0) {
            perror("Error receiving data");
            exit(1);
        } else if (reval == 0) {
            break;
        }
        fwrite(buffer, 1, reval, args->fp);
    }

    close(args->client_sock);
    fclose(args->fp);
    pthread_exit(NULL);
}

void tcp_server(const char *port) {

    struct sockaddr_storage client_addr;
    socklen_t client_addrlen = sizeof(client_addr);

    int tcp_sock = tcp_bind((struct sockaddr *)&client_addr, &client_addrlen, NULL, port);
    if (listen(tcp_sock, SOMAXCONN) < 0) {
        perror("listen fail");
    }

    pthread_t thread_id;

    while (1) {
        struct ThreadArgs args;
        args.client_sock = accept(tcp_sock, (struct sockaddr *)&client_addr, &client_addrlen);
        if (args.client_sock < 0) {
            perror("accept fail");
            break;
        }

        // Create a new thread to handle the client
        if (pthread_create(&thread_id, NULL, handle_client, (void *)&args) != 0) {
            perror("Error creating thread");
            close(args.client_sock);
            continue;
        }
        pthread_detach(thread_id);
    }

    close(tcp_sock);

    printf("File receive successfully.\n");
}