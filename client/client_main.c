//
// Created by go on 19.09.21.
//

#include "client.h"


int main(int argc, char* argv[]) {

    char* num_packets;
    if (argc > 2) printf("Usage: pl [optional: number_packets]");


    // later:
    // argv[1] is how many packets
    // argv[2] is over how many seconds

    num_packets = argc == 2 ? argv[1] : "100"; // NOLINT(cert-err34-c)

    // Step 1, establish a tcp connection with the server
    // transmit the information about how many packets I will send
    // one I've gotten acknowledge
    tcp_send("localhost", PORT, num_packets);


    // Step 2:
    // send $num_packets udp packets to server

    // Step 3:
    // wait for server to tell me how many udp packets he received

    // Step 4:
    // wait for server to tell me that he will send udp packets now

    // Step 5:
    // count how many udp packets I received

    // Step 6:
    // output all that information to the user
}