#ifndef __TLS_SERVER_H
#define __TLS_SERVER_H

#include "tcp-server.h"
#include <openssl/ssl.h>
#include <openssl/err.h>

class TlsServer : public TcpServer{
private:
    SSL_CTX *ctx_;
    SSL *ssl_;

public:
    TlsServer(char *port) : TcpServer(port){
        // OpenSSL 초기화
        SSL_library_init();
        OpenSSL_add_all_algorithms();
        SSL_load_error_strings();

        // SSL 컨텍스트 생성
        ctx_ = SSL_CTX_new(TLS_server_method());
        if (ctx_ == nullptr) {
            std::cerr << "SSL_CTX_new error." << std::endl;
            ERR_print_errors_fp(stderr);
            exit(EXIT_FAILURE);
        }
    }
    int run(char *CERT_FILE, char *KEY_FILE); // run(void) 와 다르기 때문에 override 안함
    
    ~TlsServer() {
        SSL_CTX_free(ctx_);
    }
};

#endif // __TLS_SERVER_H