cmake_minimum_required(VERSION 3.31)
project(Go C)

set(CMAKE_C_STANDARD 23)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
include_directories(${SDL2_INCLUDE_DIRS})
link_directories(${SDL2_LIBRARY_DIRS})

add_executable(Go main.c)

target_link_libraries(Go ${SDL2_LIBRARIES})