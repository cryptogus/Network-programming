#include <iostream>
#include <cstring>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <openssl/ssl.h>
#include <openssl/err.h>
// g++ -o tls_client openssl-tls-client-sample.cpp -lssl -lcrypto
#define SERVER_ADDR "127.0.0.1"
#define SERVER_PORT 4433

class TLSClient {
public:
    TLSClient() {
        SSL_library_init();
        OpenSSL_add_all_algorithms();
        SSL_load_error_strings();

        ctx_ = SSL_CTX_new(TLS_client_method());
        if (ctx_ == nullptr) {
            std::cerr << "SSL_CTX_new error." << std::endl;
            ERR_print_errors_fp(stderr);
            exit(EXIT_FAILURE);
        }
    }

    ~TLSClient() {
        SSL_CTX_free(ctx_);
    }

    void connectToServer() {
        // 소켓 생성 및 연결
        socket_ = createSocket(SERVER_ADDR, SERVER_PORT);

        // SSL 소켓 생성 및 연결
        ssl_ = createSSLConnection(socket_);

        // 데이터 전송
        const char *msg = "Hello, Server!";
        SSL_write(ssl_, msg, strlen(msg));

        // 데이터 수신
        char buffer[1024];
        int bytes_received = SSL_read(ssl_, buffer, sizeof(buffer));
        if (bytes_received > 0) {
            buffer[bytes_received] = '\0';
            std::cout << "Received: " << buffer << std::endl;
        }

        // 연결 종료
        SSL_shutdown(ssl_);
        close(socket_);
        SSL_free(ssl_);
    }

private:
    SSL_CTX *ctx_;
    SSL *ssl_;
    int socket_;

    int createSocket(const char *serverAddr, int port) {
        int clientSocket;
        struct sockaddr_in server;

        clientSocket = socket(AF_INET, SOCK_STREAM, 0);
        if (clientSocket == -1) {
            perror("Socket creation error");
            exit(EXIT_FAILURE);
        }

        memset(&server, 0, sizeof(server));
        server.sin_family = AF_INET;
        server.sin_port = htons(port);

        if (inet_pton(AF_INET, serverAddr, &server.sin_addr) <= 0) {
            perror("Invalid server address");
            close(clientSocket);
            exit(EXIT_FAILURE);
        }

        if (connect(clientSocket, (struct sockaddr *)&server, sizeof(server)) == -1) {
            perror("Connection error");
            close(clientSocket);
            exit(EXIT_FAILURE);
        }

        return clientSocket;
    }

    SSL *createSSLConnection(int socket) {
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
};

int main() {
    TLSClient client;
    client.connectToServer();

    return 0;
}
