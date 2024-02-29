#include "udp.h"

void usage(void) {
#ifdef UDPSERVER
    printf("usage: udp-server <Port> <The name of the file to save>");
#else
    printf("usage: udp-client <Ip> <Port> <File name to send>");
#endif
}
struct Info {
    const char *ip;
    const char *port;
    const char *filename;
};

int main(int argc, char *argv[]) {

    struct Info info;

#ifdef UDPSERVER
    if (argc != 3) {
        usage();
        return 1;
    }
    info.port = argv[1];
    info.filename = argv[2];
    udp_server(info.port, info.filename);
#else
    if (argc != 4) {
        usage();
        return 2;
    }
    info.ip = argv[1];
    info.port = argv[2];
    info.filename = argv[3];
    udp_client(info.ip, info.port, info.filename);
#endif
}