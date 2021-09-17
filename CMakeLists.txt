cmake_minimum_required(VERSION 3.20)
project(pl C)

set(CMAKE_C_STANDARD 11)

add_executable(client client/client.c client/client.h util/util.c util/util.h)
add_executable(server server/server.c server/server.h util/util.c util/util.h)
add_executable(listener server/listener.c server/listener.h util/util.c util/util.h)
add_executable(talker client/talker.c client/talker.h util/util.c util/util.h)
