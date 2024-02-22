#include "udp.h"
// https://www.scaler.com/topics/file-transfer-using-udp/
// https://modoocode.com/68
// https://github.com/nikhilroxtomar/File-Transfer-using-UDP-Socket-in-C
// https://www.geeksforgeeks.org/c-program-for-file-transfer-using-udp/
/***
 * Bind UDP port
 */

int udp_bind(struct sockaddr *addr, socklen_t* addrlen, const char *ip, const char *port) {
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

  if (-1 == (sock = socket(result->ai_family, SOCK_DGRAM, IPPROTO_UDP))) {
    perror("Cannot create socket");
    freeaddrinfo(result);
  }

#ifdef UDPSERVER
  if (0 != bind(sock, result->ai_addr, result->ai_addrlen)) {
    perror("Cannot bind");
    close(sock);
    freeaddrinfo(result);
  }
#endif

 return sock;
}

int udp_client(const char *ip, const char *port, const char *filepath) {
    
    FILE *fp = fopen(filepath, "rb");
    if (!fp) {
        perror("Error opening file");
    }

    struct sockaddr_storage client_addr;
    socklen_t client_addrlen = sizeof(client_addr);
    
    int udp_sock = udp_bind((struct sockaddr *)&client_addr, &client_addrlen, ip, port);
    
    char buffer[BUFSIZ];
    size_t bytesRead;

    while((bytesRead = fread(buffer, 1, sizeof(buffer), fp)) > 0) {
        printf("%ld\n", bytesRead);
         if (sendto(udp_sock, buffer, bytesRead, 0, (struct sockaddr *)&client_addr, sizeof(client_addr)) < 0) {
            perror("Error sending data");
        }
    }

    fclose(fp);
    close(udp_sock);

    printf("File sent successfully.\n");

    return 0;
}

int udp_server(const char *ip, const char *port, const char *filepath) {

    FILE *fp = fopen(filepath, "wb");
    if (!fp) {
        perror("Error opening file");
    }

    struct sockaddr_storage client_addr;
    socklen_t client_addrlen = sizeof(client_addr);
    
    int udp_sock = udp_bind((struct sockaddr *)&client_addr, &client_addrlen, ip, port);
    char buffer[BUFSIZ];

     while (1) {
        socklen_t client_len = sizeof(client_addr);
        ssize_t reval = recvfrom(udp_sock, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, &client_len);
        printf("%ld\n", reval);
        if (reval < 0) {
            perror("Error receiving data");
        } else if (reval == 0) {
            break;  // Transmission complete
        }
        fwrite(buffer, 1, reval, fp);
        if (reval < sizeof(buffer))
            break;
    }

    fclose(fp);
    close(udp_sock);
}