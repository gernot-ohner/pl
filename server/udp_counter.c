#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include "../util/util.h"
#include "udp_counter.h"

volatile sig_atomic_t keep_listening = true;

int main(void)
{
    // region declarations
    int sockfd;
    struct addrinfo *servinfo, *p;
    int rv;
    struct sockaddr_storage their_addr;
    char buf[MAX_BUF_LEN];
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];
    // endregion

    struct addrinfo hints = get_hints(AF_INET6, SOCK_DGRAM, AI_PASSIVE);

    if (Getaddrinfo(NULL, MY_PORT, &hints, &servinfo) != 0) return 1;

    // region bind to address
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
    // endregion

    // region install interrupt listener
    struct sigaction sa;
    sa.sa_handler = sig_usr1_handler;
    sa.sa_flags = 0;
    sigemptyset(&sa.sa_mask);
    sigaction(SIGUSR1, &sa, NULL);
    // endregion

    printf("listener: waiting to recvfrom...\n");
    int packet_count = 0;
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    // TO replace this for loop with testing a flag that the other process can set
    // maybe make a `volatile sig_atomic_t continue_listening?` and set that to false in a signal handler for SIGUSR1
    while (keep_listening){
        addr_len = sizeof their_addr;
        ssize_t num_bytes = recvfrom(sockfd, buf, MAX_BUF_LEN - 1, 0,
                                 (struct sockaddr*) &their_addr, &addr_len);

        if (num_bytes != -1) {
            printf("listener: got packet from %s\n",
                   inet_ntop(their_addr.ss_family,
                             get_in_addr((struct sockaddr*) &their_addr),
                             s, sizeof s));
            printf("listener: packet is %zd bytes long\n", num_bytes);
            buf[num_bytes] = '\0';
            printf("listener: packet contains \"%s\"\n", buf);
            packet_count++;
        } else {
            printf("listener [PID: %d]: no packet at the moment\n", getpid());
            sleep(1);
        }
    }

    printf("listener: counted %d packets\n", packet_count);

    printf("listener: sending packets to pipe\n");
    transfer_to_other_process(packet_count);

    close(sockfd);
    return 0;
}

ssize_t transfer_to_other_process(int packet_count) {
    mknod(FIFO_NAME, S_IFIFO | 0666, 0);
    int fd = open(FIFO_NAME, O_WRONLY);
    char digits[100];
    snprintf(digits, 100, "%d", packet_count);
    ssize_t write_result = write(fd, digits, 100);
    return write_result;
}

int sig_usr1_handler(int sig) {
    keep_listening = false;
    return 0;
}

int Recvfrom(int fd, void* buf, size_t len, int flags, struct sockaddr* server, socklen_t* server_addr_len) {
    int num_bytes;
    if ((num_bytes = recvfrom(fd, buf, len , flags, server, server_addr_len)) == -1) {
        perror("recvfrom");
        exit(1);
    }
    return num_bytes;
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
    return sockfd;
}