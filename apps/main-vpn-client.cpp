#include "tun_interface.h"

void usage(void) {
    std::cout << "Usage: vpn-client <server's public ip> <server's listening port>" << std::endl;
    std::cout << "sample: vpn-client 168.127.1.3 12345" << std::endl;
}
char *SERVER_HOST;
int main(int argc, char *argv[]) {

    if (argc != 3) {
        usage();
        return 1;
    }
    SERVER_HOST = argv[1];
    Tun_Interface VPN_client(argv[1], argv[2]);

    if (VPN_client.run() == -1) {
        perror("running error");
        //std::cerr << "Error running." << std::endl;
        return 2;
    }
    //std::atexit(client.~UDP_client());
    return 0;
}