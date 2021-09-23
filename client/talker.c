/*
** talker.c -- a datagram "client" demo
*/

#include "talker.h"
#include "../util/util.h"

int main(int argc, char *argv[])
{
    int sockfd;
    struct addrinfo *servinfo, *p;
    int rv;
    int num_bytes;

    if (argc != 3) {
        fprintf(stderr,"usage: talker hostname message\n");
        exit(1);
    }

    struct addrinfo hints = get_hints(AF_INET6, SOCK_DGRAM, 0);

    if (Getaddrinfo(argv[1], SERVER_PORT, &hints, &servinfo) != 0) return 1;

    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next) {
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

    if ((num_bytes = sendto(sockfd, argv[2], strlen(argv[2]), 0,
                           p->ai_addr, p->ai_addrlen)) == -1) {
        perror("talker: sendto");
        exit(1);
    }

    freeaddrinfo(servinfo);

    printf("talker: sent %d bytes to %s\n", num_bytes, argv[1]);
    close(sockfd);

    return 0;
}