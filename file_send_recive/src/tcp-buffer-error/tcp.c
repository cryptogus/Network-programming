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

static char* extract_name(const char* filename) {

    const char* last_slash = strrchr(filename, '/');

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

    char buffer[1040];
    size_t bytesRead;

    const char* name = extract_name(filename);
    char len = (char)strlen(name);
    send(tcp_sock, &len, 1, 0);
    char check[2];
    int reval = recv(tcp_sock,  check, sizeof(check), 0);
    send(tcp_sock, name, strlen(name) + 1, 0);
    reval = recv(tcp_sock,  check, sizeof(check), 0);
    int i = 0;// debug
    while((bytesRead = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        i++;
        printf("byteread %dth = %ld byte\n", i, bytesRead);
        if (send(tcp_sock, buffer, bytesRead, 0) < 0) {
            perror("Error sending data");
        }
        reval = recv(tcp_sock,  check, sizeof(check), 0);
        if (reval < 0) {
            perror("The server didn't receive all the data");
        }
    }

    fclose(fp);
    close(tcp_sock);

    printf("File sent successfully.\n");
}
#ifdef TCPSERVER

void tcp_server(const char *port) {

    struct sockaddr_storage client_addr;
    socklen_t client_addrlen = sizeof(client_addr);

    int tcp_sock = tcp_bind((struct sockaddr *)&client_addr, &client_addrlen, NULL, port);
    if (listen(tcp_sock, SOMAXCONN) < 0) {
        perror("listen fail");
    }
    char check[3] = "ok";
    //while (1) { test 용이라 accept는 무한 루프 해제함
        
        int client_sock = accept(tcp_sock, (struct sockaddr *)&client_addr, &client_addrlen);
        if (client_sock < 0) {
            perror("accept fail");
            //break;
        }
        
        char buffer[1024];
        recv(client_sock, buffer, sizeof(buffer), 0);
        // 잘 받았다고 응답
        send(client_sock, check, strlen(check), 0);
        char filename[buffer[0] + 1];
        recv(client_sock, filename, sizeof(filename), 0);
        send(client_sock, check, strlen(check), 0);

        FILE *fp = fopen(filename, "wb");
        if (!fp) {
            perror("Error opening file");
        }
        int i = 0; //debug
        while (1) {
            memset(buffer, 0, sizeof(buffer));
            ssize_t reval = recv(client_sock, buffer, sizeof(buffer), 0);
            if (reval < 0) {
                perror("Error receiving data");
                exit(1);
            } else if (reval == 0) {
                break;
            }else {
                i++;
                printf("reval %dth = %ld byte\n", i, reval);
                fwrite(buffer, 1, reval, fp);
                send(client_sock, check, strlen(check), 0);
            }
        }

        close(client_sock);
        fclose(fp);
        printf("File receive successfully.\n");
    //}

    close(tcp_sock);
}
#endif