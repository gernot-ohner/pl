# `pl` is a very simple packet loss manager

There is a server and a client
- The server will be listening for [[tcp]] connections.
- The client will connect to the server via tcp and tell it:
    - I'm going to send you 100 [[UDP]] packets over the next 5 seconds.
    - Please listen out for them and count how many arrived.
    - Afterwards please tell me how many arrived.
- The server will say:
    - I received x UDP packets from you
    - I will also send you 100 UDP packets.
- Then the client will count how many UDP packets arrived from the server
  and present the information:
  loss on the way there, loss on the way back, total loss

## Rationale
I built this project to familiarize myself with
1. Linux syscalls, particularly: UDP and TCP sockets and interprocess communication
2. fork-join parallelism