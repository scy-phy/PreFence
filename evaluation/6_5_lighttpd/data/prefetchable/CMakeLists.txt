cmake_minimum_required(VERSION 3.7)

# set up project
set(PROJ_NAME "prefetchable")
project(${PROJ_NAME} LANGUAGES CXX)

# add debug symbols to all outputs
set(CMAKE_BUILD_TYPE Debug)

# collect source files
file(GLOB SOURCES "src/*.cc")

# define resulting binary
add_executable(${PROJ_NAME} ${SOURCES})

# COMPILER FLAGS
# enable warnings
target_compile_options(${PROJ_NAME} PRIVATE "-Wall")
# enable C++17 support
target_compile_options(${PROJ_NAME} PRIVATE "-std=c++17")
