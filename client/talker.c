/*
** talker.c -- a datagram "client" demo
*/

#include "talker.h"
#include "../util/util.h"

int udp_send(int num_packets) {
    int sockfd;
    struct addrinfo* servinfo, * p;
    int rv;
    int num_bytes;

    struct addrinfo hints = get_hints(AF_INET, SOCK_DGRAM, 0);

    // TODO fix the hostname
    if (Getaddrinfo(SERVER_NAME, SERVER_PORT, &hints, &servinfo) != 0) return 1;

    // loop through all the results and make a socket
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("talker: socket");
            continue;
        }

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "talker: failed to create socket\n");
        return 2;
    }

    // TODO what do I do about the message?
    for (int i = 0; i < num_packets; i++) {
        if ((num_bytes = sendto(sockfd, "hi", 2, 0,
                                p->ai_addr, p->ai_addrlen)) == -1) {
            perror("talker: sendto");
            exit(1);
        }
    }

    freeaddrinfo(servinfo);

    printf("talker: sent %d bytes to %s\n", num_bytes, SERVER_NAME);
    close(sockfd);

    return 0;
}