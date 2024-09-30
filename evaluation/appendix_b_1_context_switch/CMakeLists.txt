cmake_minimum_required(VERSION 3.7)

# set up project
set(PROJ_NAME "context-measure")
project(${PROJ_NAME} LANGUAGES CXX)

# add debug symbols to all outputs
set(CMAKE_BUILD_TYPE Debug)

# define resulting binary
add_executable(${PROJ_NAME}-from src/main-from.cc)
add_executable(${PROJ_NAME}-to-set src/main-to-set.cc)
add_executable(${PROJ_NAME}-to-clear src/main-to-clear.cc)

# COMPILER FLAGS
# enable warnings
target_compile_options(${PROJ_NAME}-from PRIVATE "-Wall")
target_compile_options(${PROJ_NAME}-to-set PRIVATE "-Wall")
target_compile_options(${PROJ_NAME}-to-clear PRIVATE "-Wall")
# enable C++17 support
target_compile_options(${PROJ_NAME}-from PRIVATE "-std=c++17")
target_compile_options(${PROJ_NAME}-to-set PRIVATE "-std=c++17")
target_compile_options(${PROJ_NAME}-to-clear PRIVATE "-std=c++17")
