cmake_minimum_required(VERSION 2.6)
project(coco)

include_directories(./src)

add_executable(client client.cpp)
add_executable(server server.cpp)

target_link_libraries(client coco pthread)
target_link_libraries(server coco pthread)

