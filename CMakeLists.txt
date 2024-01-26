cmake_minimum_required(VERSION 3.10)

project(security-network LANGUAGES CXX)
#set(SERVER "")
set(TCP_SERVER ${CMAKE_CURRENT_LIST_DIR}/src/tcp-server.cpp)
set(TCP_CLIENT ${CMAKE_CURRENT_LIST_DIR}/src/tcp-client.cpp)
set(UDP_SERVER ${CMAKE_CURRENT_LIST_DIR}/src/udp_server.cpp)
set(UDP_CLIENT ${CMAKE_CURRENT_LIST_DIR}/src/udp_client.cpp)

# add_subdirectory(src) -> library 만들까?
add_subdirectory(tests)

include_directories(header)

add_executable(tcp-server ${TCP_SERVER} apps/main_tcp_server.cpp)
target_compile_options(tcp-server PRIVATE -Wall -Wextra -pthread) # -g) 디버깅 옵션 주석처리

add_executable(tcp-client ${TCP_CLIENT} apps/main_tcp_client.cpp)
target_compile_options(tcp-client PRIVATE -Wall -Wextra) # -g) 디버깅 옵션 주석처리

add_executable(udp-server ${UDP_SERVER} apps/main-udp-server.cpp)
target_compile_options(tcp-server PRIVATE -Wall -Wextra -pthread) # -g) 디버깅 옵션 주석처리

add_executable(udp-client ${UDP_CLIENT} apps/main-udp-client.cpp)
target_compile_options(tcp-client PRIVATE -Wall -Wextra) # -g) 디버깅 옵션 주석처리