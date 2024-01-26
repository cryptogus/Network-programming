#include "udp_client.h"

void usage(void) {
    std::cout << "udp-client <ip> <port>" << std::endl;
}

int main(int argc, char *argv[]) {

    if (argc != 3) {
        usage();
        return 1;
    }
    
    UDP_client client(argv[1], argv[2]);

    if (client.run() == -1) {
        perror("running error");
        //std::cerr << "Error running." << std::endl;
        return 2;
    }
    //std::atexit(client.~UDP_client());
    return 0;
}