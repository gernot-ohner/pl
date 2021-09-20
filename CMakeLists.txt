cmake_minimum_required(VERSION 3.20)
project(pl C)

set(CMAKE_C_STANDARD 11)

#add_executable(client client/client.c client/client.h util/util.c util/util.h)
#add_executable(server server/server.c server/server.h util/util.c util/util.h)
#add_executable(listener server/listener.c server/listener.h util/util.c util/util.h)
#add_executable(talker client/talker.c client/talker.h util/util.c util/util.h)
add_executable(client_main
        client/client_main.c
        client/client_main.h
        client/client.c
        client/client.h
        util/util.c
        util/util.h)

add_executable(server_main
        server/server_main.c
        server/server_main.h
        server/server.c
        server/server.h
        util/util.c
        util/util.h)
