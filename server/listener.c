/*
** listener.c -- a datagram sockets "server" demo
*/

#include "listener.h"
#include "../util/util.h"

int main(void)
{
    int sockfd;
    struct addrinfo *servinfo, *p;
    int rv;
    struct sockaddr_storage their_addr;
    char buf[MAXBUFLEN];
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];

    struct addrinfo hints = get_hints(AF_INET6, SOCK_DGRAM, AI_PASSIVE);

    if (Getaddrinfo(NULL, MYPORT, &hints, &servinfo) != 0) return 1;

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }

        if (Bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) continue;

        break;
    }

    if (p == NULL) {
        fprintf(stderr, "listener: failed to bind socket\n");
        return 2;
    }

    freeaddrinfo(servinfo);

    printf("listener: waiting to recvfrom...\n");

    addr_len = sizeof their_addr;
    int numbytes = Recvfrom(sockfd, buf, MAXBUFLEN-1 , 0,
             (struct sockaddr *)&their_addr, &addr_len);

    printf("listener: got packet from %s\n",
           inet_ntop(their_addr.ss_family,
                     get_in_addr((struct sockaddr *)&their_addr),
                     s, sizeof s));
    printf("listener: packet is %d bytes long\n", numbytes);
    buf[numbytes] = '\0';
    printf("listener: packet contains \"%s\"\n", buf);

    close(sockfd);

    return 0;
}

int Recvfrom(int fd, void* buf, size_t len, int flags, struct sockaddr* server, socklen_t* server_addlen) {
    int numbytes;
    if ((numbytes = recvfrom(fd, buf, len , flags, server, server_addlen)) == -1) {
        perror("recvfrom");
        exit(1);
    }
    return numbytes;
}

int Bind(int fd, const struct sockaddr* addr, socklen_t len) {
    if (bind(fd, addr, len) == -1) {
        close(fd);
        perror("listener: bind");
        return -1;
    }
    return 0;
}

int Socket(int domain, int type, int protocol) {
    int sockfd;
    if ((sockfd = socket(domain, type, protocol)) == -1) {
        perror("listener: socket");
        return -1;
    }
    return 0;
}