#include "udp_server.h"

/***
 * Setting UDP information
*/
UDP_server::UDP_server(char *port) {

    // Initialization
    memset(&hints_, 0, sizeof(hints_));
    
    // UDP protocol
    hints_.ai_family = AF_UNSPEC;    // IPv4 or IPv6 (The host does not specify which IPv4 or IPv6 addresses to use)
    hints_.ai_socktype = SOCK_DGRAM;
    hints_.ai_protocol = IPPROTO_UDP;
    char ip[] = "0.0.0.0";           // or ::, INADDR_ANY, in6addr_any, <value>.sin_addr.s_addr = htonl(INADDR_ANY);

    // Get ip address information (ip is DNS or string ip(ex, "127.0.0.1"))
    int status;
    if ((status = getaddrinfo(ip, NULL, &hints_, &result_)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    }
    
    // Setting port ----- Not ip version !!
    if (result_->ai_family == AF_INET) { // if ipv4
        ((struct sockaddr_in *)result_->ai_addr)->sin_port = htons(std::stoi(port));
    } else if (result_->ai_family == AF_INET6) { // if ipv6
        ((struct sockaddr_in6 *)result_->ai_addr)->sin6_port = htons(std::stoi(port));
    } else {
        fprintf(stderr, "unknown ai_family %s", strerror(errno));
    }

    // Create socket
    sock_ = socket(result_->ai_family, hints_.ai_socktype, hints_.ai_protocol);
    if (sock_ == -1) {
        fprintf(stderr, "socket creating error: %s\n", strerror(errno));
    }
    // Bind to server socket (ip version & port number, protocol)
    int retval = bind(sock_, result_->ai_addr, result_->ai_addrlen);
    if (retval == -1) {
        fprintf(stderr, "%s\n", strerror(errno));
        close(sock_);
    }
    /* Very important 정보 복사 안해서 오류떳음 */
    memcpy((struct sockaddr *)&client_addr, result_->ai_addr, result_->ai_addrlen);
    client_addrlen = result_->ai_addrlen;

    freeaddrinfo(result_);
}

/***
 * UDP server run 
*/

int UDP_server::run(void) {
    
    // Networking to client
    char addr[INET_ADDRSTRLEN];
    char buffer[BUFSIZ + 1];
    while(true) {
        // Receive data from client
        int retval = recvfrom(sock_, buffer, BUFSIZ, 0, (sockaddr *)&client_addr, &client_addrlen);
        if (retval == -1) {
            fprintf(stderr, "recvfrom is error, %s\n", strerror(errno));
            break;
        }
        
        buffer[retval] = '\0';
        // inet_ntop(client_addr.ss_family, &(((sockaddr_in *)&client_addr)->sin_addr), addr, INET_ADDRSTRLEN);
        getnameinfo((struct sockaddr *)&client_addr, client_addrlen, addr, sizeof(addr), NULL, 0, NI_NUMERICHOST);
        printf("[UDP %s:%d] %s\n", addr, ntohs(((sockaddr_in *)&client_addr)->sin_port), buffer);

        // sendto()
        retval = sendto(sock_, buffer, retval, 0, (sockaddr *)&client_addr, client_addrlen);
        if (retval == -1) {
            fprintf(stderr, "sendto is error, %s\n", strerror(errno));
            break;
        }
        
    }

    close(sock_);

    return 1;
}