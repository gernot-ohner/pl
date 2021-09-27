//
// Created by go on 17.09.21.
//

#ifndef PL_UDP_COUNTER_H
#define PL_UDP_COUNTER_H

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

#define MY_PORT "4950"	// the port users will be connecting to
#define FIFO_NAME "ipc"
#define OUT

#define MAX_BUF_LEN 100

int udp_listen();

static int Bind(int fd, const struct sockaddr* addr, socklen_t len);


static ssize_t transfer_to_other_process(int packet_count);

static void sig_usr1_handler(__attribute__((unused)) int sig);

#endif //PL_UDP_COUNTER_H
