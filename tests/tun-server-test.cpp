#include "tun_interface.h"

char *LAN_network_device;

int main(void) {
    LAN_network_device = "eth0";
    Tun_Interface VPN_server("12345", LAN_network_device); // instead of br-lan for test
    VPN_server.run();
}