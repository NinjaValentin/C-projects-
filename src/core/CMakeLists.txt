cmake_minimum_required(VERSION 3.10.2)	

project(haffman_core)

file(GLOB_RECURSE CORE "c/*.c")

add_library(haffman_core STATIC ${CORE})