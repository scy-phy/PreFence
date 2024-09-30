cmake_minimum_required(VERSION 3.13.4)

set(PROJ_NAME "victim-bn")

project(${PROJ_NAME} LANGUAGES C)

# add debug symbols
set(CMAKE_BUILD_TYPE Debug)

# define main executables
add_executable(${PROJ_NAME} "src/victim-bn.c")

# define helper executable
add_executable(expected_locations "src/expected_locations.c")

# compile position-indep. binary
# enable warnings
target_compile_options(${PROJ_NAME} PRIVATE "-Wall")
target_compile_options(${PROJ_NAME} PRIVATE "-fPIE")
target_compile_options(${PROJ_NAME} PRIVATE "-fno-plt")

# include openssl
include_directories("../openssl/include")
file(GLOB OPENSSL_LIBCRYPTO "../openssl/libcrypto.so.1.1")
target_link_libraries(${PROJ_NAME} ${OPENSSL_LIBCRYPTO})
