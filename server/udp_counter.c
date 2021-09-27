#include <fcntl.h>
#include <sys/stat.h>
#include <signal.h>
#include "../util/util.h"
#include "udp_counter.h"

volatile sig_atomic_t keep_listening = true;

int udp_listen() {
    // region declarations
    int sockfd;
    struct addrinfo* servinfo, * p;
    int rv;
    struct sockaddr_storage their_addr;
    char buf[MAX_BUF_LEN];
    socklen_t addr_len;
    char s[INET6_ADDRSTRLEN];
    // endregion

    struct addrinfo hints = get_hints(AF_INET, SOCK_DGRAM, AI_PASSIVE);

    if (Getaddrinfo(NULL, MY_PORT, &hints, &servinfo) != 0) return -1;

    // region bind to address
    // loop through all the results and bind to the first we can
    for (p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1) {
            perror("listener: socket");
            continue;
        }
        if (Bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) continue;
        break;
    }
    if (p == NULL) {
        fprintf(stderr, "listener [PID: %d]: failed to bind socket\n", getpid());
        return -1;
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

    printf("listener [PID: %d]: waiting to recvfrom...\n", getpid());
    int packet_count = 0;
    fcntl(sockfd, F_SETFL, O_NONBLOCK);
    while (keep_listening) {
        // TODO this is pretty bad design.
        //   if the client dies or is cancelled after it told the server to listen
        //   but before it told the server to stop, the server is going to loop forever
        //   SO: I need to add a fallback mechanism to stop the loop

        addr_len = sizeof their_addr;
        printf("listener [PID %d]: trying to read from fd %d\n", getpid(), sockfd);
        ssize_t num_bytes = recvfrom(sockfd, buf, MAX_BUF_LEN - 1, 0,
                                     OUT (struct sockaddr*) &their_addr, &addr_len);

        if (num_bytes != -1) {
            buf[num_bytes] = '\0';
            printf("listener [PID %d]: got packet from %s, contains: %s (%zd) bytes\n",
                   getpid(),
                   inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr*) &their_addr), s, sizeof s),
                   buf,
                   num_bytes);
            packet_count++;
        } else {
            printf("listener [PID: %d]: no packet at the moment\n", getpid());
            sleep(1);
        }
    }

    printf("listener [PID: %d]: counted %d packets\n", getpid(), packet_count);
    printf("listener [PID: %d]: sending packets to pipe\n", getpid());
    if (transfer_to_other_process(packet_count) == -1) return -1;

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

void sig_usr1_handler(__attribute__((unused)) int sig) {
    keep_listening = false;
}

int Bind(int fd, const struct sockaddr* addr, socklen_t len) {
    if (bind(fd, addr, len) == -1) {
        close(fd);
        perror("listener: bind");
        return -1;
    }
    return 0;
}
