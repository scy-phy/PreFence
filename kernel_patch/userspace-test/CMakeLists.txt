cmake_minimum_required(VERSION 3.13.4)

set(PROJ_NAME "pf-dis-user")

project(${PROJ_NAME} LANGUAGES CXX)

# add debug symbols
set(CMAKE_BUILD_TYPE Debug)

# define main executables
add_executable(${PROJ_NAME} "src/main.cc")

# enable warnings
target_compile_options(${PROJ_NAME} PRIVATE "-Wall")

#set_target_properties(${PROJ_NAME} PROPERTIES COMPILE_FLAGS "-m32" LINK_FLAGS "-m32")
