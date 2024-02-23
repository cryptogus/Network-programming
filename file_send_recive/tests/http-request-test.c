#include "tcp.h"

int main(int argc, char *argv[]) {
    // example.com == 93.184.216.34
    // http_request("93.184.216.34", "80", "GET / HTTP/1.1\r\n\r\n");
    // http_request("93.184.216.34", "80", "GET / HTTP/1.1\r\nHost : example.com\r\nConnection: keep-alive\r\n\r\n");
    
    // man.openbsd.org == 128.100.17.244 (using ping man.openbsd.org)
    http_request("128.100.17.244", "80", "GET /nc.1 HTTP/1.1\r\nHost: man.openbsd.org\r\n\r\n");

    return 0;
}