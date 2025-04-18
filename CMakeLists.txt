cmake_minimum_required(VERSION 3.30)
project(cpp_projekt_internal)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SRC src/*.cc src/*.hh)

add_library(cpp_projekt_internal SHARED ${SRC})