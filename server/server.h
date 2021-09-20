//
// Created by go on 17.09.21.
//

#ifndef PL_SERVER_H
#define PL_SERVER_H


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include "../util/util.h"

#define PORT "3490"  // the port users will be connecting to

#define BACKLOG 10	 // how many pending connections queue will hold

static void sigchld_handler(int s);
int tcp_server(void);

#endif //PL_SERVER_H
