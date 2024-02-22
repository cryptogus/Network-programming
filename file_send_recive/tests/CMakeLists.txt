include_directories(../src/udp ../src/tcp)

add_executable(udp-client-test ../src/udp/udp.c udp-client-test.c)
add_executable(udp-server-test ../src/udp/udp.c udp-server-test.c)
target_compile_definitions(udp-server-test PRIVATE UDPSERVER)

add_executable(tcp-client-test ../src/tcp/tcp.c tcp-client-test.c)
add_executable(tcp-server-test ../src/tcp/tcp.c tcp-server-test.c)
target_compile_definitions(tcp-server-test PRIVATE TCPSERVER)