#include "udp_server.h"

void usage(void) {
    std::cout << "udp-server <port>" << std::endl;
}

int main(int argc, char *argv[]) {

    if (argc != 2) {
        usage();
        return 1;
    }

    UDP_server server(argv[1]);

    if (server.run() == -1) {
        perror("running error");
        //std::cerr << "Error running." << std::endl;
        return 2;
    }
    
    return 0;
}