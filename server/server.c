#include <sys/stat.h>
#include <fcntl.h>
#include "server.h"
#include "udp_counter.h"

int tcp_server(void)
{
    //region setup
    int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
    struct addrinfo *servinfo, *p;
    struct sockaddr_storage their_addr; // connector's address information
    socklen_t sin_size;
    struct sigaction sa;
    int yes=1;
    char s[INET6_ADDRSTRLEN];

    struct addrinfo hints = get_hints(AF_UNSPEC, SOCK_STREAM, AI_PASSIVE);
    //endregion

    //region Establish Connection
    // I don't need the server argument here?
    if (Getaddrinfo(NULL, PORT, &hints, &servinfo) != 0) return 1;

    // loop through all the results and bind to the first we can
    for(p = servinfo; p != NULL; p = p->ai_next) {
        if ((sockfd = socket(p->ai_family, p->ai_socktype,
                             p->ai_protocol)) == -1) {
            perror("server: socket");
            continue;
        }

        if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
                       sizeof(int)) == -1) {
            perror("setsockopt");
            exit(1);
        }

        if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
            close(sockfd);
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(servinfo); // all done with this structure

    if (p == NULL)  {
        fprintf(stderr, "server [PID %d]: failed to bind\n", getpid());
        exit(1);
    }

    if (listen(sockfd, BACKLOG) == -1) {
        perror("listen");
        exit(1);
    }
//    endregion

    //region signal handling
    sa.sa_handler = sigchld_handler; // reap all dead processes
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = SA_RESTART;
    if (sigaction(SIGCHLD, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
    //endregion

    printf("server [PID: %d]: waiting for connections...\n", getpid());

    while(1) {  // tcp_receive accept() loop
        sin_size = sizeof their_addr;
        new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
        if (new_fd == -1) {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
        printf("server [PID: %d]: got connection from %s\n", getpid(), s);

        pid_t child_pid = fork();

        char udpMsg[100];
        char interruptUdp[10]; // TODO ask dave about "may point to memory out of scope"
        char receivedBuf[100]; //  if I move this inside the loop

        if (!child_pid) { // this is the child process
            close(sockfd); // child doesn't need the listener

            printf("server [PID %d]: trying to receive from fd %d\n", getpid(), new_fd);
            const ssize_t bytes_received = recv(new_fd, receivedBuf, 100, 0);
            printf("server [PID: %d]: received %zd bytes of data: %s\n", getpid(), bytes_received, receivedBuf);

            pid_t udp_pid = fork();
            printf("server [PID: %d]: forked off process %d\n", getpid(), udp_pid);
            if (udp_pid == 0) { // this is the child
                udp_listen("localhost", 10, udp_pid);
                exit(0);
                // TODO I still need to harvest these orphan children somehow!

//                const ssize_t bytes_sent_about_udp = send(new_fd, udpMsg, strlen(udpMsg), 0);
//                if (bytes_sent_about_udp == -1) perror("udp send");
//                else printf("server [PID: %d]: sent %ld bytes of data: %s\n", getpid(), bytes_sent_about_udp, udpMsg);
            } else {
                printf("server [PID: %d]: waiting for signal to interrupt %d now\n", getpid(), udp_pid);
                recv(new_fd, interruptUdp, 100, 0);
                if (!strcmp(interruptUdp, "interrupt")) {
                    printf("server [PID: %d]: killing process %d\n", getpid(), udp_pid);
                    int kill_result = kill(udp_pid, SIGUSR1);
                    int packets_counted = readFromOtherProcess();

                    snprintf(udpMsg, 100, "server: udp listener reported %d packets counted", packets_counted);
                    printf("server [PID: %d]: udp listener reported %d packets counted\n", getpid(), packets_counted);

                    if (kill_result != -1) {
                        printf("server [PID: %d]: successfully sent the signal!\n", getpid());
                    } else {
                        printf("server [PID: %d]: failed to send the signal!\n", getpid());
                    }
                } else {
                    snprintf(udpMsg, 100, "server: could not count packets\n");
                    printf("server [PID %d]: received packet with unexpected content: %s", getpid(), interruptUdp);
                }

                // report the number of received packets back to the client
                ssize_t send_successful = send(new_fd, udpMsg, 100, 0);
                if (send_successful != -1) {
                    close(new_fd);
                    exit(0);
                } else {
                    close(new_fd);
                    perror("send udp count");
                    exit(1);
                }
            }
        }



        close(new_fd);  // parent doesn't need this
    }

#pragma clang diagnostic push
#pragma ide diagnostic ignored "UnreachableCode"
    return 0;
#pragma clang diagnostic pop
}

int readFromOtherProcess() {
    char buf[MAX_BUF_LEN];

    mknod(FIFO_NAME, S_IFIFO | 0666, 0);
    int fd = open(FIFO_NAME, O_RDONLY);

    if (read(fd, buf, MAX_BUF_LEN) == - 1) {
        perror("pipe read");
        exit(1);
    }

//    int packets_counted = atoi(buf);
    // todo check if that works
    int packets_counted = (int) strtol(buf, NULL, 10);

    return packets_counted;
}


static void sigchld_handler(int s)
{
    (void)s; // quiet unused variable warning

    // waitpid() might overwrite errno, so we save and restore it:
    int saved_errno = errno;

    while(waitpid(-1, NULL, WNOHANG) > 0);

    errno = saved_errno;
}
