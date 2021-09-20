//
// Created by go on 17.09.21.
//

#ifndef PL_LISTENER_H
#define PL_LISTENER_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#define MYPORT "4950"	// the port users will be connecting to

#define MAXBUFLEN 100

int Recvfrom(int fd, void* buf, size_t len, int flags, struct sockaddr* server, socklen_t* server_addlen);
int Bind(int fd, const struct sockaddr* addr, socklen_t len);
int Socket(int domain, int type, int protocol);

#endif //PL_LISTENER_H
