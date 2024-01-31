#ifndef __TUN_INTERFACE_H
#define __TUN_INTERFACE_H

#include "tun_alloc.h"
#include "lea.h"

#ifdef AS_CLIENT
#include "udp_client.h"
class Tun_Interface : public UDP_client{
public:
    Tun_Interface(char *SERVER_PUBLIC_IP, char *SERVER_PORT) : UDP_client(SERVER_PUBLIC_IP, SERVER_PORT){};
    int tun_fd, udp_fd;
    void flags(void);
    int run(void) override final;
};
#else
#include "udp_server.h"
class Tun_Interface : public UDP_server{

public:
    Tun_Interface(char *port) : UDP_server(port){};
    int tun_fd, udp_fd;
    void flags(void);
    int run(void) override final;
};
#endif
#endif