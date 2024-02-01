#include "tun_interface.h"

char *SERVER_HOST;

int main(void) {
    SERVER_HOST = "127.0.0.1";
    Tun_Interface VPN_client(SERVER_HOST, "12345");
    VPN_client.run();
}