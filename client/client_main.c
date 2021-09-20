//
// Created by go on 19.09.21.
//

#include "client_main.h"
#include "client.h"


int main(int argc, char* argv[]) {

    // later:
    // argv[1] is how many packets
    // argv[2] is over how many seconds
    // actually, scratch the second one and always make it 5 seconds
    int num_packets = 100;
    int seconds = 5;

    // first, establish a tcp connection with the server
    // transmit the information about how many packets I will send
    // one I've gotten acknowledge
    tcp_send("localhost", 3490, "Hello World!");
}