#include "udp.h"

int main(int argc, char *argv[]) {
    if (mkdir("receive", 0755) == -1)
        perror("mkdir fail");
    udp_server("12345", "./receive/recieve_test.txt");

    return 0;
}