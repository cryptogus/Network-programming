#ifndef __TCP_H
#define __TCP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h> 

void tcp_client(const char *ip, const char *port, const char *filename);
void tcp_server(const char *port);

struct ThreadArgs {
    int client_sock;
    FILE *fp;
};
#endif //__TCP_H