#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
// g++ -o tls_server openssl-tls-server-sample.cpp -lssl -lcrypto
// openssl req -x509 -nodes -days 365 -newkey rsa:3092 -keyout server.key -out server.crt

#define SERVER_PORT 4433
#define CERT_FILE "server.crt"
#define KEY_FILE "server.key"

int create_server_socket(int port) {
    int server_socket;
    struct sockaddr_in server_addr;

    // 소켓 생성
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        perror("Socket creation error");
        return -1;
    }

    // 서버 주소 설정
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(port);

    // 소켓과 주소 바인딩
    if (bind(server_socket, reinterpret_cast<struct sockaddr*>(&server_addr), sizeof(server_addr)) == -1) {
        perror("Binding error");
        close(server_socket);
        return -1;
    }

    // 연결 대기
    if (listen(server_socket, 10) == -1) {
        perror("Listen error");
        close(server_socket);
        return -1;
    }

    return server_socket;
}

int main() {
    SSL_CTX *ctx;
    SSL *ssl;
    int server, client;

    // OpenSSL 초기화
    SSL_library_init();
    OpenSSL_add_all_algorithms();
    SSL_load_error_strings();

    // SSL 컨텍스트 생성
    ctx = SSL_CTX_new(TLS_server_method());
    if (ctx == nullptr) {
        std::cerr << "SSL_CTX_new error." << std::endl;
        ERR_print_errors_fp(stderr);
        return 1;
    }

    // 인증서와 키 로드
    if (SSL_CTX_use_certificate_file(ctx, CERT_FILE, SSL_FILETYPE_PEM) <= 0) {
        std::cerr << "Error loading certificate from file." << std::endl;
        ERR_print_errors_fp(stderr);
        return 1;
    }

    if (SSL_CTX_use_PrivateKey_file(ctx, KEY_FILE, SSL_FILETYPE_PEM) <= 0) {
        std::cerr << "Error loading private key from file." << std::endl;
        ERR_print_errors_fp(stderr);
        return 1;
    }

    // 서버 소켓 생성 및 연결 대기
    server = create_server_socket(SERVER_PORT);
    if (server < 0) {
        std::cerr << "Server socket creation error." << std::endl;
        return 1;
    }

    // 클라이언트 연결 대기
    client = accept(server, nullptr, nullptr);
    if (client == -1) {
        perror("Accept error");
        close(server);
        return 1;
    }

    // SSL 소켓 생성
    ssl = SSL_new(ctx);
    SSL_set_fd(ssl, client);

    // SSL 핸드셰이크 수행
    if (SSL_accept(ssl) == -1) {
        std::cerr << "SSL_accept error." << std::endl;
        ERR_print_errors_fp(stderr);
        close(client);
        close(server);
        return 1;
    }

    // 데이터 수신
    char buffer[1024];
    int bytes_received = SSL_read(ssl, buffer, sizeof(buffer));
    if (bytes_received > 0) {
        buffer[bytes_received] = '\0';
        std::cout << "Received: " << buffer << std::endl;

        // 데이터 전송
        const char *msg = "Hello, Client!";
        SSL_write(ssl, msg, strlen(msg));
    }

    // 연결 종료
    SSL_shutdown(ssl);
    close(client);
    close(server);
    SSL_free(ssl);
    SSL_CTX_free(ctx);

    return 0;
}
