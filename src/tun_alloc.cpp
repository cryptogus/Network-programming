#include "tun_alloc.h"

extern char *SERVER_HOST;
extern char *LAN_network_device;
/*
 * Create VPN interface /dev/tun0 and return a fd
 */
int tun_alloc() {
  struct ifreq ifr;
  int fd, e;

  if ((fd = open("/dev/net/tun", O_RDWR)) < 0) {
    perror("Cannot open /dev/net/tun");
    return fd;
  }

  memset(&ifr, 0, sizeof(ifr));

  ifr.ifr_flags = IFF_TUN | IFF_NO_PI;
  strncpy(ifr.ifr_name, "tun0", IFNAMSIZ);

  if ((e = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0) {
    perror("ioctl[TUNSETIFF]");
    close(fd);
    return e;
  }

  return fd;
}
/****
 * If you run the server and client together, you'll crash the tun device.
 * You should use this comment function and replace some tun0s with tun1s in other functions.
 * 

int tun_alloc() {
  struct ifreq ifr;
  int fd, e;

  if ((fd = open("/dev/net/tun", O_RDWR)) < 0) {
    perror("Cannot open /dev/net/tun");
    return fd;
  }

  memset(&ifr, 0, sizeof(ifr));

  ifr.ifr_flags = IFF_TUN | IFF_NO_PI;
#ifdef AS_CLIENT
  strncpy(ifr.ifr_name, "tun1", IFNAMSIZ);
#else
  strncpy(ifr.ifr_name, "tun0", IFNAMSIZ);
#endif
  if ((e = ioctl(fd, TUNSETIFF, (void *) &ifr)) < 0) {
    perror("ioctl[TUNSETIFF]");
    close(fd);
    return e;
  }

  return fd;
}

*/


/*
 * Configure IP address and MTU of VPN interface /dev/tun0
 */
void ifconfig() {
  char cmd[1024];

#ifdef AS_CLIENT
  snprintf(cmd, sizeof(cmd), "ifconfig tun0 10.8.0.2 netmask 255.255.0.0 mtu %d up", MTU);
#else
  snprintf(cmd, sizeof(cmd), "ifconfig tun0 10.8.0.1 netmask 255.255.0.0 mtu %d up", MTU);
#endif
  system(cmd);
}

/*
 * Setup route table via `iptables` & `ip route`
 */
void setup_route_table() {
  system("sysctl -w net.ipv4.ip_forward=1");

#ifdef AS_CLIENT
  //system("iptables -t nat -A POSTROUTING -o tun0 -j MASQUERADE");
  system("iptables -I FORWARD 1 -i tun0 -m state --state RELATED,ESTABLISHED -j ACCEPT");
  system("iptables -I FORWARD 1 -o tun0 -j ACCEPT");
  char cmd[1024];
  snprintf(cmd, sizeof(cmd), "ip route add %s via $(ip route show 0/0 | sed -e 's/.* via \\([^ ]*\\).*/\\1/')", SERVER_HOST);
  system(cmd);
  system("ip route add 0/1 dev tun0");
  system("ip route add 128/1 dev tun0");
#else
  
  char cmd[1024];
  char cmd2[1024];
  snprintf(cmd, sizeof(cmd), "iptables -A FORWARD -i tun0 -o %s -j ACCEPT", LAN_network_device);
  system(cmd);
  snprintf(cmd2, sizeof(cmd2), "iptables -A FORWARD -i %s -o tun0 -m conntrack --ctstate RELATED,ESTABLISHED -j ACCEPT", LAN_network_device);
  system(cmd2);
  
#endif
}

/*
 * Cleanup route table
 */
void cleanup_route_table() {
#ifdef AS_CLIENT
  //system("iptables -t nat -D POSTROUTING -o tun0 -j MASQUERADE");
  system("iptables -D FORWARD -i tun0 -m state --state RELATED,ESTABLISHED -j ACCEPT");
  system("iptables -D FORWARD -o tun0 -j ACCEPT");
  char cmd[1024];
  snprintf(cmd, sizeof(cmd), "ip route del %s", SERVER_HOST);
  system(cmd);
  system("ip route del 0/1");
  system("ip route del 128/1");
#else
  char cmd[1024];
  char cmd2[1024];
  snprintf(cmd, sizeof(cmd), "iptables -D FORWARD -i tun0 -o %s -j ACCEPT", LAN_network_device);
  system(cmd);
  snprintf(cmd2, sizeof(cmd2), "iptables -D FORWARD -i %s -o tun0 -m conntrack --ctstate RELATED,ESTABLISHED -j ACCEPT", LAN_network_device);
  system(cmd2);
#endif
}
/*
 * Catch Ctrl-C and `kill`s, make sure route table gets cleaned before this process exit
 */
void cleanup(int signo) {
  printf("He's really going for the art when he goes.\n");
  if (signo == SIGHUP || signo == SIGINT || signo == SIGTERM) {
    cleanup_route_table();
    exit(0);
  }
}

void cleanup_when_sig_exit() {
  struct sigaction sa;
  sa.sa_handler = &cleanup;
  sa.sa_flags = SA_RESTART;
  sigfillset(&sa.sa_mask);

  if (sigaction(SIGHUP, &sa, NULL) < 0) {
    perror("Cannot handle SIGHUP");
  }
  if (sigaction(SIGINT, &sa, NULL) < 0) {
    perror("Cannot handle SIGINT");
  }
  if (sigaction(SIGTERM, &sa, NULL) < 0) {
    perror("Cannot handle SIGTERM");
  }
}