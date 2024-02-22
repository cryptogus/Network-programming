#include "tcp.h"

int main(int argc, char *argv[]) {
    tcp_server("0.0.0.0", "12340", "/home/hhlee/wallpadtest/tests/recieve/recieve_test.txt");

    return 0;
}