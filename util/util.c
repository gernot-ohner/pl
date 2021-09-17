//
// Created by go on 16.09.21.
//

#include <netdb.h>
#include "util.h"

void* get_in_addr(struct sockaddr* sa) {
    if (sa->sa_family == AF_INET) {
        return &(((struct sockaddr_in*) sa)->sin_addr); // this is from <netinet/in.h>
    } else {
        return &(((struct sockaddr_in6*) sa)->sin6_addr);
    }
}

struct addrinfo get_hints(int family, int type, int flags) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = family;
    hints.ai_socktype = type;
    hints.ai_flags = flags;
    return hints;
}
