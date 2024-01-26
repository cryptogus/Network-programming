#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/if.h>
#include <linux/if_tun.h>

#define TUN_DEV "/dev/net/tun"

int tun_alloc(char *dev) {
    struct ifreq ifr;
    int fd, err;

    if ((fd = open(TUN_DEV, O_RDWR)) < 0) {
        perror("Opening /dev/net/tun");
        return fd;
    }

    memset(&ifr, 0, sizeof(ifr));

    ifr.ifr_flags = IFF_TUN | IFF_NO_PI;  // IFF_NO_PI: Do not provide packet information

    if (*dev) {
        strncpy(ifr.ifr_name, dev, IFNAMSIZ);
    }

    if ((err = ioctl(fd, TUNSETIFF, (void *)&ifr)) < 0) {
        perror("ioctl(TUNSETIFF)");
        close(fd);
        return err;
    }

    strcpy(dev, ifr.ifr_name);

    return fd;
}

int main() {
    char tun_name[IFNAMSIZ] = "tun0";
    int tun_fd;

    // Allocate TUN device
    tun_fd = tun_alloc(tun_name);

    if (tun_fd < 0) {
        fprintf(stderr, "Error creating TUN device\n");
        return 1;
    }

    printf("Allocated TUN device: %s\n", tun_name);

    // Now you can read/write to tun_fd like a regular file descriptor

    // For example, reading from TUN device
    char buffer[1500];
    ssize_t nbytes = read(tun_fd, buffer, sizeof(buffer));

    if (nbytes < 0) {
        perror("Error reading from TUN device");
        close(tun_fd);
        return 1;
    }

    printf("Read %zd bytes from TUN device\n", nbytes);

    // Handling the received packet goes here

    // Remember to close the TUN device when done
    close(tun_fd);

    return 0;
}
