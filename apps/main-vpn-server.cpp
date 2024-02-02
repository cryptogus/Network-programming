#include "tun_interface.h"

void usage(void) {
    std::cout << "Usage: vpn-server <listening port> <LAN networ device>" << std::endl;
    std::cout << "sample: vpn-server 12345 br-lan" << std::endl;
}
char *LAN_network_device;
int main(int argc, char *argv[]) {

    if (argc != 3) {
        usage();
        return 1;
    }
    LAN_network_device = argv[2];
    Tun_Interface VPN_server(argv[1], argv[2]);

    if (VPN_server.run() == -1) {
        perror("running error");
        //std::cerr << "Error running." << std::endl;
        return 2;
    }
    //std::atexit(client.~UDP_client());
    return 0;
}