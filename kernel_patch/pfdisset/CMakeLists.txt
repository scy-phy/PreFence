cmake_minimum_required(VERSION 3.13.4)

set(PROJ_NAME "pfdisset")

project(${PROJ_NAME} LANGUAGES C)

# add debug symbols
set(CMAKE_BUILD_TYPE Debug)

# define main executables
add_executable(${PROJ_NAME} "src/main.c")

# enable warnings
target_compile_options(${PROJ_NAME} PRIVATE "-Wall")
