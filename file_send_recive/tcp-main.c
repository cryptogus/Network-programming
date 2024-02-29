#include "tcp.h"

void usage(void) {
#ifdef TCPSERVER
    printf("usage: tcp-server <Port>\n");
#else
    printf("usage: tcp-client <Ip> <Port> <File name to send>\n");
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
    if (argc != 2) {
        usage();
        return 1;
    }
    info.port = argv[1];
    tcp_server(info.port);
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