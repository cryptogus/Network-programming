#include "tcp-client.h"
#include <openssl/ssl.h>
#include <openssl/err.h>

class TlsClient : public TcpClient{
private:
    SSL_CTX *ctx_;
    SSL *ssl_;
    int socket_;
    SSL *createSSLConnection(int socket);
public:
    TlsClient(char *ip, char *port) : TcpClient(ip, port){
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

};