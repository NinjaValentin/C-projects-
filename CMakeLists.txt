cmake_minimum_required(VERSION 3.10.2)	

project(console_huffman_archiver)
set(SOURCE_EXE main.c)

include_directories(src/core/header)

add_executable(archiver ${SOURCE_EXE})

add_subdirectory(src/core/)				

target_link_libraries(archiver haffman_core)	