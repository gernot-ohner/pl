/*
** client.c -- a stream socket client demo
*/

#include "client.h"
#include "../util/util.h"


int tcp_send(char serverName[], const char* port, char message[])
{
    //region setup
    int sockfd;
    char buf[MAX_DATA_SIZE];
    struct addrinfo *servinfo, *p;
    char s[INET6_ADDRSTRLEN];

    struct addrinfo hints = get_hints(AF_UNSPEC, SOCK_STREAM, 0);

    if (Getaddrinfo(serverName, port, &hints, &servinfo) != 0) return 1;
    //endregion

    //region Establish Connection
    // loop through all the results and connect to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("client: socket");
            continue;
        }

        if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            perror("client: connect");
            close(sockfd);
            continue;
        }

        break;
    }
    if (p == NULL) {
        fprintf(stderr, "client: failed to connect\n");
        return 2;
    }
    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
              s, sizeof s);
    printf("client: connecting to %s\n", s);
    freeaddrinfo(servinfo); // all done with this structure
    //endregion

    //region Send
    char* setUpBuf = message;
    ssize_t bytes_sent = send(sockfd, setUpBuf, strlen(setUpBuf), 0);
    printf("sent %zd bytes\n", bytes_sent);
    //endregion

    //region Receive
    ssize_t num_bytes = Recv(sockfd, buf);
    buf[num_bytes] = '\0';
    printf("client: received '%s'\n",buf);
    //endregion

    //region finally
    close(sockfd);
    return 0;
    //endregion
}

ssize_t Recv(int sockfd, char* buf) {
    ssize_t num_bytes;
    if ((num_bytes = recv(sockfd, buf, MAX_DATA_SIZE - 1, 0)) == -1) {
        perror("recv");
        exit(1);
    }
    return num_bytes;
}
