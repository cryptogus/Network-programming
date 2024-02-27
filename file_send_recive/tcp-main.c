#include "tcp.h"

void usage(void) {
#ifdef TCPSERVER
    printf("usage: tcp-server <Port> <The name of the file to save>");
#else
    printf("usage: tcp-client <Ip> <Port> <File name to send>");
#endif
}

struct Info {
    const char *ip;
    const char *port;
    const char *filename;
};

int main(int argc, char *argv[]) {

    struct Info info;
    
#ifdef TCPSERVER
    if (argc != 3) {
        usage();
        return 1;
    }
    info.port = argv[1];
    info.filename = argv[2];
    tcp_server(info.port, info.filename);
#else
    if (argc != 4) {
        usage();
        return 2;
    }
    info.ip = argv[1];
    info.port = argv[2];
    info.filename = argv[3];
    tcp_client(info.ip, info.port, info.filename);
#endif
}