cmake_minimum_required(VERSION 3.11)

project(wallpad-test LANGUAGES C)

option(CROSS_COMPILE "on/off cross compile" OFF)
# set(CMAKE_TOOLCHAIN_FILE toolchain_rasppi.cmake) #CMAKE_TOOCHAIN_FILE 변수는 파일을 지정만 하면 해당 파일을 알아서 include
############### 컴파일러 및 툴체인 지정 ###############
if (${CROSS_COMPILE})
    set(CMAKE_SYSTEM_NAME Linux)
    set(CMAKE_SYSTEM_PROCESSOR armhf)
    set(CMAKE_C_COMPILER arm-linux-gnueabihf-gcc)
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
endif()
#####################################################
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

if(${CROSS_COMPILE})
    target_include_directories(https-request PRIVATE libcurl/include libopenssl/include)
    target_link_directories(https-request PRIVATE libcurl/lib libopenssl/lib)
    target_link_libraries(https-request PRIVATE ssl crypto curl)
    target_compile_options(https-request PRIVATE "-Wl,-rpath, libopenssl/lib")
else()
    find_package(CURL REQUIRED)
    target_include_directories(https-request PRIVATE ${CURL_INCLUDE_DIRS})
    target_link_libraries(https-request PRIVATE ${CURL_LIBRARIES})
endif()