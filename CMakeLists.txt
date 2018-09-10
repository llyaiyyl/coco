cmake_minimum_required(VERSION 2.6)
project(coco)

include_directories(./src)
aux_source_directory(./src DIR_SRCS)

add_library(coco ${DIR_SRCS})

add_subdirectory(./example)
