include_directories(../header/)
add_executable(tcp_server_test ../src/tcp-server.cpp server-test.cpp)
add_executable(tcp_client_test ../src/tcp-client.cpp client-test.cpp)