#include "tcp.h"

int main(int argc, char *argv[]) {
    if (mkdir("receive", 0755) == -1)
        perror("mkdir fail");
    tcp_server("12340", "./receive/recieve_test.txt");

    return 0;
}