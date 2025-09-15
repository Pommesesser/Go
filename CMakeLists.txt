cmake_minimum_required(VERSION 3.16)
project(Go C)
set(CMAKE_C_STANDARD 23)

find_package(SDL2 REQUIRED CONFIG)
add_executable(Go main.c)
target_link_libraries(Go PRIVATE SDL2::SDL2)