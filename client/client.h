//
// Created by go on 17.09.21.
//

#ifndef PL_CLIENT_H
#define PL_CLIENT_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#define PORT "3490" // the port client will be connecting to

#define MAX_DATA_SIZE 100 // max number of bytes we can get at once

ssize_t Recv(int sockfd, char* buf);

int tcp_send(char serverName[], const char* port, char message[]);


#endif //PL_CLIENT_H
