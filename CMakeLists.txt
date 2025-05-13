cmake_minimum_required(VERSION 3.30)
project(cpp_projekt_internal)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static-libgcc -static-libstdc++ -static")
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -static -Wl,--subsystem,windows,--kill-at,--enable-stdcall-fixup")

file(GLOB_RECURSE SRC src/*.cc src/*.hh lib/*.cc lib/*.hh)
include_directories(lib)

add_library(cpp_projekt_internal SHARED ${SRC})