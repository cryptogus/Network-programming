#ifndef __TUN_INTERFACE_H
#define __TUN_INTERFACE_H

#include "lea.h"

extern char *SERVER_HOST;
extern char *LAN_network_device;

#ifdef AS_CLIENT
#include "udp_client.h"
class Tun_Interface : public UDP_client{
public:
    Tun_Interface(char *VPN_SERVER_PUBLIC_IP, char *VPN_SERVER_PORT) : UDP_client(VPN_SERVER_PUBLIC_IP, VPN_SERVER_PORT){
        SERVER_HOST = VPN_SERVER_PUBLIC_IP;
    };
    int tun_fd, udp_fd;
    void flags(void);
    int run(void) override final;
};
#else
#include "udp_server.h"
class Tun_Interface : public UDP_server{

public:
    Tun_Interface(char *VPN_SERVER_PORT, char * brlan_device) : UDP_server(VPN_SERVER_PORT){
        LAN_network_device = brlan_device;
    };
    int tun_fd, udp_fd;
    void flags(void);
    int run(void) override final;
};
#endif
#endif