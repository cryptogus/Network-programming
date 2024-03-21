#include "tls_server.h"

int TlsServer::run(char *CERT_FILE, char *KEY_FILE) {
    // 인증서와 키 로드
    if (SSL_CTX_use_certificate_file(ctx_, CERT_FILE, SSL_FILETYPE_PEM) <= 0) {
        std::cerr << "Error loading certificate from file." << std::endl;
        ERR_print_errors_fp(stderr);
        return 1;
    }

    if (SSL_CTX_use_PrivateKey_file(ctx_, KEY_FILE, SSL_FILETYPE_PEM) <= 0) {
        std::cerr << "Error loading private key from file." << std::endl;
        ERR_print_errors_fp(stderr);
        return 1;
    }
    listen_sock_ = socket(addr.sin_family, SOCK_STREAM, 0);
    if (listen_sock_ == -1) {
        perror("Socket creation error");
        return -1;
    }

    if (bind(listen_sock_, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
        perror("Socket bind error");
        return -1;
    }

    if (listen(listen_sock_, SOMAXCONN) == -1) {
        fprintf(stderr, "Socket bind error\n");
        return -1;
    }

    while(true) {
        // 클라이언트 연결 대기
        socklen_t addrlen = sizeof(client_addr);
        client_sock_ = accept(listen_sock_, (struct sockaddr *)&client_addr, &addrlen);
        if (client_sock_ == -1) {
            perror("accept error");
            return -1;
        }
    
        // SSL 소켓 생성
        ssl_ = SSL_new(ctx_);
        SSL_set_fd(ssl_, client_sock_);

        // SSL 핸드셰이크 수행
        if (SSL_accept(ssl_) == -1) {
            std::cerr << "SSL_accept error." << std::endl;
            ERR_print_errors_fp(stderr);
            close(client_sock_);
            close(listen_sock_);
            return 1;
        }

        // 데이터 수신
        char buffer[1024];
        while(true) {
            int bytes_received = SSL_read(ssl_, buffer, sizeof(buffer));
            if (bytes_received > 0) {
                buffer[bytes_received] = '\0';
                std::cout << "Received: " << buffer << std::endl;

                // 데이터 전송
                const char *msg = "---->I'm tls server and always saying: Hello, Client!";
                SSL_write(ssl_, msg, strlen(msg));
            }
            memset(buffer, 0, strlen(buffer) + 1);
            
        }
    }
    // 연결 종료
    SSL_shutdown(ssl_);
    close(client_sock_);
    close(listen_sock_);
    SSL_free(ssl_);
    SSL_CTX_free(ctx_);
    return 0;
}