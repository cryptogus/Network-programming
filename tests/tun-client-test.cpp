#include "tun_interface.h"

char *SERVER_HOST;

int main(void) {
    SERVER_HOST = "122.211.166.156"; // server public ip
    Tun_Interface VPN_client(SERVER_HOST, "12345");
    VPN_client.run();
}