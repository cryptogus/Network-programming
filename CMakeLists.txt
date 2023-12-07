cmake_minimum_required(VERSION 3.10)

project(security-network LANGUAGES CXX)
set(SERVER "")
set(SERVER ${CMAKE_CURRENT_LIST_DIR}/src/tcp-server.cpp)

# add_subdirectory(src) -> library 만들까?
add_subdirectory(tests)

include_directories(header)

add_executable(tcp-server ${SERVER} apps/main_tcp_server.cpp)
target_compile_options(tcp-server PRIVATE -Wall -Wextra) # -g) 디버깅 옵션 주석처리