#include "udp.h"

int main(int argc, char *argv[]) {
    udp_client("127.0.0.1", "12345", "/home/hhlee/wallpadtest/tests/test.txt");

    return 0;
}