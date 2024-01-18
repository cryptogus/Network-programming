#include "udp_server.h"

void usage(void) {
    std::cout << "udp_server_test <port>" << std::endl;
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        usage();
    }

    UDP_server server(argv[1]);

    if (server.run() == -1) {
        std::cout << "running error\n";
        return 2;
    }
    
    return 0;
}