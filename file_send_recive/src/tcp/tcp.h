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
#include <syslog.h>
// #include <syslog.h>
#include <sys/stat.h> // mkdir func
// #include <setjmp.h> // instead of goto

void tcp_client(const char *ip, const char *port, const char *filepath);
void tcp_server(const char *port, const char *filepath);
#endif //__TCP_H