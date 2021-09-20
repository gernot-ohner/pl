//
// Created by go on 17.09.21.
//

#ifndef PL_TALKER_H
#define PL_TALKER_H

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

#define SERVER_PORT "4950"	// the port users will be connecting to

int Sendto(char* const* argv, int sockfd, const struct addrinfo* p);

#endif //PL_TALKER_H
