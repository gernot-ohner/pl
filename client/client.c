/*
** client.c -- a stream socket client demo
*/

#include "client.h"
#include "../util/util.h"


int tcp_open(char serverName[], const char* port)
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
        fprintf(stderr, "client [PID: %d]: failed to connect\n", getpid());
        exit(1);
    }

    inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
              s, sizeof s);
    printf("client [PID: %d]: connecting to %s\n", getpid(), s);
    freeaddrinfo(servinfo); // all done with this structure
    //endregion



    return sockfd;
}

ssize_t Recv(int sockfd, char* buf) {
    printf("client [PID: %d]: trying to receive on fd %d", getpid(), sockfd);
    ssize_t num_bytes;
    if ((num_bytes = recv(sockfd, buf, MAX_DATA_SIZE - 1, 0)) == -1) {
        perror("recv");
        exit(1);
    }
    return num_bytes;
}

ssize_t tcp_send(int fd, char* msg) {

    //region Send
    char* setUpBuf = msg;
    ssize_t bytes_sent = send(fd, setUpBuf, strlen(setUpBuf), 0);
    if (bytes_sent == -1) {
        perror("mysend");
        return -1;
    }
    printf("client [PID: %d]: sent %zd bytes\n", getpid(), bytes_sent);
    //endregion

    return bytes_sent;
}

ssize_t tcp_receive(int fd, OUT char* msg) {

    //region Send
    ssize_t bytes_received = recv(fd, msg, 100, 0);
    if (bytes_received == -1) {
        perror("myreceive");
        return -1;
    }
    printf("client [PID: %d]: received %zd bytes\n", getpid(), bytes_received);
    //endregion

    return bytes_received;
}

