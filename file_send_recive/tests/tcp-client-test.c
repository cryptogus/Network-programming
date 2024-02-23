#include "tcp.h"

int main(int argc, char *argv[]) {
    tcp_client("127.0.0.1", "12340", "test.txt");
    return 0;
}