include_directories(../src/udp ../src/tcp)

add_executable(udp-client-test ../src/udp/udp.c udp-client-test.c)
add_executable(udp-server-test ../src/udp/udp.c udp-server-test.c)
target_compile_definitions(udp-server-test PRIVATE UDPSERVER)

add_executable(tcp-client-test ../src/tcp/tcp.c tcp-client-test.c)
add_executable(tcp-server-test ../src/tcp/tcp.c tcp-server-test.c)
target_compile_definitions(tcp-server-test PRIVATE TCPSERVER)
target_compile_options(tcp-server-test PUBLIC -lpthread)

add_executable(http-request-test ../src/tcp/tcp.c http-request-test.c)
# COPY test files to build directory
file(COPY send DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# file(COPY test.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# file(COPY 30mb_test.txt DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
# file(COPY recieve DESTINATION ${CMAKE_CURRENT_BINARY_DIR}) directory copy, not used
