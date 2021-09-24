cmake_minimum_required(VERSION 3.20)
project(pl C)

set(CMAKE_C_STANDARD 11)

add_executable(client_main
        client/client_main.c
        client/client.c
        client/client.h
        util/util.c
        util/util.h)

add_executable(server_main
        server/server_main.c
        server/server.c
        server/server.h
        util/util.c
        util/util.h)

add_executable(udp_counter
        server/udp_counter.c
        server/udp_counter.h
        util/util.c
        util/util.h)

add_executable(udp_talker
        client/talker.c
        client/talker.h
        util/util.c
        util/util.c)

add_executable(guide
        guide/signal.c
        guide/pipes.c)

add_executable(speak guide/speak.c)
add_executable(tick guide/tick.c)