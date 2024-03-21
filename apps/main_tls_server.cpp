#include "tls_server.h"

void usage(char *name) {
    std::cout << name << " <port> <CERT FILE> <KEY FILE>" << std::endl;
}

int main(int argc, char *argv[]) {

    if (argc != 4) {
        usage(argv[0]);
        return 1;
    }

    TlsServer Tlsserver(argv[1]);
   
    if (Tlsserver.run(argv[2], argv[3]) == -1) {
        perror("running error");
        //std::cerr << "Error running." << std::endl;
        return 2;
    }
    
    return 0;
}