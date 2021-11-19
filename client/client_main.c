//
// Created by go on 19.09.21.
//

#include "client.h"
#include "talker.h"


int main(int argc, char* argv[]) {

    char* num_packets;
    int bytes_sent;
    if (argc > 2) printf("Usage: pl [optional: number_packets]");


    // later:
    // argv[1] is how many packets
    // argv[2] is over how many seconds

    num_packets = argc == 2 ? argv[1] : "100"; // NOLINT(cert-err34-c)

    // Step 1, establish a tcp connection with the server
    // transmit the information about how many packets I will send
    // one I've gotten acknowledge
    // NOT A GOOD ABSTRACTION: should be more like "establish connection" & send on established connection
    int fd = tcp_open(SERVER_NAME, PORT);
    printf("client [PID: %d]: sending initial TCP packet to fd %d\n", getpid(), fd);
    bytes_sent = tcp_send(fd, "100");
    if (bytes_sent == -1) exit(1);
    printf("client [PID: %d]: sent initial TCP packet to fd %d\n", getpid(), fd);


    // Step 2:
    // send $num_packets udp packets to server

    sleep(2);
    printf("client [PID: %d]: sending UDP packets\n", getpid());
    udp_send(100);
    printf("client [PID: %d]: sent UDP packets\n", getpid());

    sleep(2);
    // Step 3:
    // wait for server to tell me how many udp packets he received
    printf("client [PID: %d]: sending \"interrupt\" TCP packet to fd %d\n", getpid(), fd);
    bytes_sent = tcp_send(fd, "interrupt");
    if (bytes_sent == -1) exit(1);
    printf("client [PID: %d]: sent \"interrupt\" TCP packet to fd %d\n", getpid(), fd);

    // Step 4:
    // wait for server to tell me that he will send udp packets now
    char response[100];
    tcp_receive(fd, response);
    printf("client [PID %d]: got response: \"%s\"\n", getpid(), response);


    // Step 5:
    // count how many udp packets I received

    // Step 6:
    // output all that information to the user
}