#include <time.h>
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

    struct timespec packet_spacing_time;
    packet_spacing_time.tv_sec = 0;
    packet_spacing_time.tv_nsec = 5000000000 / num_packets;
    struct timespec remaining_time;

    int packet_sent_counter = 0;
    printf("client [PID %d]: sent packets:\n", getpid());
    for (int i = 0; i < num_packets; i++) {
        if ((num_bytes = sendto(sockfd, "hi", 2, 0,
                                p->ai_addr, p->ai_addrlen)) == -1) {
            perror("talker: sendto");
            exit(1);
        }
        packet_sent_counter++;
        printf("*");
        fflush(stdout);
        nanosleep(&packet_spacing_time, &remaining_time);
    }
    printf("\n");
    printf("client [PID %d]: sent %d packets\n", getpid(), packet_sent_counter);

    freeaddrinfo(servinfo);

    printf("talker: sent %d bytes to %s\n", num_bytes, SERVER_NAME);
    close(sockfd);

    return 0;
}