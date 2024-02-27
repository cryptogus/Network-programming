cmake_minimum_required(VERSION 3.11)

project(wallpad-test LANGUAGES C)
# set(CMAKE_TOOLCHAIN_FILE toolchain_rasppi.cmake)
add_subdirectory(tests)

set(tcp src/tcp/tcp.c)
set(udp src/udp/udp.c)
set(https src/https/https.c)

add_executable(tcp-client ${tcp} tcp-main.c)
target_include_directories(tcp-client PUBLIC src/tcp)

add_executable(tcp-server ${tcp} tcp-main.c)
target_include_directories(tcp-server PUBLIC src/tcp)
target_compile_definitions(tcp-server PRIVATE TCPSERVER)
target_compile_options(tcp-server PUBLIC -lpthread)

add_executable(udp-client ${udp} udp-main.c)
target_include_directories(udp-client PUBLIC src/udp)

add_executable(udp-server ${udp} udp-main.c)
target_include_directories(udp-server PUBLIC src/udp)
target_compile_definitions(udp-server PRIVATE UDPSERVER)

add_executable(https-request ${https} https-request.c)
target_include_directories(https-request PUBLIC src/https)
find_package(CURL REQUIRED)
target_include_directories(https-request PRIVATE ${CURL_INCLUDE_DIRS})
target_link_libraries(https-request PRIVATE ${CURL_LIBRARIES})