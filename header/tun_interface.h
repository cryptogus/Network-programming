#ifndef __TUN_INTERFACE_H
#define __TUN_INTERFACE_H

#include "tun_alloc.h"
#include "lea.h"

#ifdef AS_CLIENT
#include "udp_client.h"
class Tun_Interface : public UDP_client{
public:
    Tun_Interface(char *VPN_SERVER_PUBLIC_IP, char *VPN_SERVER_PORT) : UDP_client(VPN_SERVER_PUBLIC_IP, VPN_SERVER_PORT){};
    int tun_fd, udp_fd;
    void flags(void);
    int run(void) override final;
};
#else
#include "udp_server.h"
class Tun_Interface : public UDP_server{

public:
    Tun_Interface(char *VPN_SERVER_PORT) : UDP_server(VPN_SERVER_PORT){};
    int tun_fd, udp_fd;
    void flags(void);
    int run(void) override final;
};
#endif
#endif