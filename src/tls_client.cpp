#include "tls_client.h"

SSL *TlsClient::createSSLConnection(int socket) {
    SSL *ssl;
    ssl = SSL_new(ctx_);
    
    SSL_set_fd(ssl, socket);
    
    if (SSL_connect(ssl) == -1) {
        std::cerr << "SSL_connect error." << std::endl;
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        exit(EXIT_FAILURE);
    }
    
    return ssl;
}

int TlsClient::run(void) {
    sock_ = socket(addr.sin_family, SOCK_STREAM, 0);
    if (sock_ == -1) {
        perror("Socket creation error");
        return -1;
    }
    int retval = 0;
    retval = connect(sock_, (struct sockaddr *)&addr, sizeof(addr));
    if (retval == -1) {
        perror("Connection fail:");
        return -1;
    }
    // SSL 소켓 생성 및 연결
    ssl_ = createSSLConnection(sock_);
    
    // 서버에 보낼 데이터
    while(true) {
        std::cout << "Enter data to send\n" << std::endl;
        std::string buf = "";
        std::getline(std::cin, buf);
        if (buf == "")
            break;
         
        // 데이터 보내기
        SSL_write(ssl_, buf.c_str(), buf.length());

        // 데이터 수신
        char buffer[1024];
        int bytes_received = SSL_read(ssl_, buffer, sizeof(buffer));
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            std::cout << "Received: " << buffer << std::endl;
        }
        memset(buffer, 0, strlen(buffer) + 1);
    }
    // 연결 종료
    SSL_shutdown(ssl_);    
    SSL_free(ssl_);
    close(sock_);
    return 0;
}