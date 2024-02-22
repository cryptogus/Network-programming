#include "udp.h"

int main(int argc, char *argv[]) {
    udp_server("0.0.0.0", "12345", "/home/hhlee/wallpadtest/tests/recieve/recieve_test.txt");

    return 0;
}