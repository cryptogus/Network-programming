#ifndef __TUN_ALLOC_H
#define __TUN_ALLOC_H

#include <stdio.h>
#include <sys/ioctl.h>
#include <cstring>
#include <signal.h>
#include <fcntl.h>
#include <linux/if_tun.h>
#include <linux/if.h>

#include <cstdlib> // for system func

#define MTU 1400

/*
 * Create VPN interface /dev/tun0 and return a fd
 */
int tun_alloc(void);

/*
 * Configure IP address and MTU of VPN interface /dev/tun0
 */
void ifconfig(void);

void setup_route_table(char *SERVER_HOST, char *LAN_network_device);
void cleanup_route_table(char *SERVER_HOST, char *LAN_network_device);

void cleanup_when_sig_exit(void);
#endif