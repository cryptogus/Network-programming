#include "udp_client.h"

UDP_client::UDP_client(char *ip, char *port) {
     // Initialization
    memset(&hints_, 0, sizeof(hints_));
    
    // UDP protocol
    hints_.ai_family = AF_UNSPEC;    // IPv4 or IPv6 (The host does not specify which IPv4 or IPv6 addresses to use)
    hints_.ai_socktype = SOCK_DGRAM;
    hints_.ai_protocol = IPPROTO_UDP;

    // Get ip address information (ip is DNS or string ip(ex, "127.0.0.1"))
    int status;
    if ((status = getaddrinfo(ip, NULL, &hints_, &result_)) != 0) {
        fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    }
    
    // Setting ip version & port
    if (result_->ai_family == AF_INET) {// ipv4
        ((struct sockaddr_in *)&server_addr)->sin_port = htons(std::stoi(port));
        ((struct sockaddr_in *)&server_addr)->sin_family = AF_INET;
        struct sockaddr_in *addr = (struct sockaddr_in *)result_->ai_addr;
        ((struct sockaddr_in *)&server_addr)->sin_addr = addr->sin_addr;
    } else if (result_->ai_family == AF_INET6) {// ipv6
        ((struct sockaddr_in6 *)&server_addr)->sin6_port = htons(std::stoi(port));
        ((struct sockaddr_in6 *)&server_addr)->sin6_family = AF_INET6;
        struct sockaddr_in6 *addr = (struct sockaddr_in6 *)result_->ai_addr;
        ((struct sockaddr_in6 *)&server_addr)->sin6_addr = addr->sin6_addr;
    } else {
        fprintf(stderr, "unknown ai_family %s", strerror(errno));
    }
    
    // Create socket
    sock_ = socket(result_->ai_family, hints_.ai_socktype, hints_.ai_protocol);
    if (sock_ == -1) {
        fprintf(stderr, "socket creating error: %s\n", strerror(errno));
    }
    server_addrlen = result_->ai_addrlen;

    freeaddrinfo(result_);
    
}

int UDP_client::run(void) {

    // Networking to server
    
    while (true) {
        std::cout << "[input text]: ";
        std::string buf = "";
        std::getline(std::cin, buf);
        if (buf == "") {
            break;
        }

        int retval = sendto(sock_, buf.c_str(), buf.length(), 0, (const sockaddr *)&server_addr, server_addrlen);
        if (retval == -1) {
            fprintf(stderr, "sendto is error, %s\n", strerror(errno));
            break;
        }

        retval = recvfrom(sock_, this->buf, BUFSIZ, 0, (sockaddr *)&server_addr, &server_addrlen);
        printf("%d\n",retval);
        if (retval == -1) {
            fprintf(stderr, "sendto is error, %s\n", strerror(errno));
            break;
        }
        
        buf[retval] = '\0';
        printf("reply: %s\n", this->buf);
        memset(this->buf, 0, BUFSIZ + 1);
    }

    close(sock_);

    return 1;
}