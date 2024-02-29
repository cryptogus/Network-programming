#include "tcp.h"

int main(int argc, char *argv[]) {
    // if (mkdir("receive", 0755) == -1)
    //     perror("mkdir fail");
    tcp_server("12340");

    return 0;
}