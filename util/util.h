//
// Created by go on 16.09.21.
//

#include <netinet/in.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>

void* get_in_addr(struct sockaddr* sa);
struct addrinfo get_hints(int family, int type, int flags);


