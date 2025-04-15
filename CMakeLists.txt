cmake_minimum_required(VERSION 3.30)
project(cpp_projekt)

set(CMAKE_CXX_STANDARD 23)

file(GLOB_RECURSE SRC src/*.cc src/*.hh libs/*.h libs/*.cpp)

add_executable(cpp_projekt ${SRC})

target_link_libraries(cpp_projekt PRIVATE dwmapi
        d3d9
        d3dcompiler
        dxguid
        user32
        gdi32
        shell32
)