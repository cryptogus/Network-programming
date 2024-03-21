cmake_minimum_required(VERSION 3.10)

project(security-network LANGUAGES CXX)

option(CROSS_COMPILE "on/off cross compile" OFF)

set(TCP_SERVER ${CMAKE_CURRENT_LIST_DIR}/src/tcp-server.cpp)
set(TCP_CLIENT ${CMAKE_CURRENT_LIST_DIR}/src/tcp-client.cpp)
set(TLS_SERVER ${CMAKE_CURRENT_LIST_DIR}/src/tls_server.cpp ${TCP_SERVER})
set(TLS_CLIENT ${CMAKE_CURRENT_LIST_DIR}/src/tls_client.cpp ${TCP_CLIENT})
set(UDP_SERVER ${CMAKE_CURRENT_LIST_DIR}/src/udp_server.cpp)
set(UDP_CLIENT ${CMAKE_CURRENT_LIST_DIR}/src/udp_client.cpp)
set(TUN ${CMAKE_CURRENT_LIST_DIR}/src/lea.cpp ${CMAKE_CURRENT_LIST_DIR}/src/tun_alloc.cpp ${CMAKE_CURRENT_LIST_DIR}/src/tun_interface.cpp)

# add_subdirectory(src) -> library 만들까?
add_subdirectory(tests)

include_directories(header)

add_executable(tcp-server ${TCP_SERVER} apps/main_tcp_server.cpp)
target_compile_options(tcp-server PRIVATE -Wall -Wextra -pthread) # -g) 디버깅 옵션 주석처리

add_executable(tcp-client ${TCP_CLIENT} apps/main_tcp_client.cpp)
target_compile_options(tcp-client PRIVATE -Wall -Wextra) # -g) 디버깅 옵션 주석처리

add_executable(udp-server ${UDP_SERVER} apps/main-udp-server.cpp)
target_compile_options(udp-server PRIVATE -Wall -Wextra) # -g) 디버깅 옵션 주석처리

add_executable(udp-client ${UDP_CLIENT} apps/main-udp-client.cpp)
target_compile_options(udp-client PRIVATE -Wall -Wextra) # -g) 디버깅 옵션 주석처리

add_executable(vpn-server ${UDP_SERVER} ${TUN} apps/main-vpn-server.cpp)
target_compile_options(vpn-server PRIVATE -Wall -Wextra)

add_executable(vpn-client ${UDP_CLIENT} ${TUN} apps/main-vpn-client.cpp)
target_compile_options(vpn-client PRIVATE -Wall -Wextra -DAS_CLIENT)

# server.crt 파일 복사
configure_file(
    ${CMAKE_SOURCE_DIR}/server.crt
    ${CMAKE_BINARY_DIR}/server.crt
    COPYONLY
)

# server.key 파일 복사
configure_file(
    ${CMAKE_SOURCE_DIR}/server.key
    ${CMAKE_BINARY_DIR}/server.key
    COPYONLY
)


# 사용자 정의 옵션: USE_OPENSSL
option(USE_OPENSSL "Use OpenSSL for TLS" ON)

if (USE_OPENSSL)
    add_executable(tls-server ${TLS_SERVER} apps/main_tls_server.cpp)
    target_compile_options(tls-server PRIVATE -Wall -Wextra) # -g) 디버깅 옵션 주석처리

    add_executable(tls-client ${TLS_CLIENT} apps/main_tls_client.cpp)
    target_compile_options(tls-client PRIVATE -Wall -Wextra) # -g) 디버깅 옵션 주석처리
endif()

# OpenSSL을 사용하는 경우
if(USE_OPENSSL AND NOT CROSS_COMPILE)
    # OpenSSL을 찾습니다.
    # REQUIRED 키워드가 사용되었기 때문에 OpenSSL을 찾을 수 없으면 CMake는 에러를 발생시키고 빌드를 중단합니다.
    find_package(OpenSSL REQUIRED)

    if(OPENSSL_FOUND)
        message(STATUS "OpenSSL found. Version: ${OPENSSL_VERSION}")
        include_directories(${OPENSSL_INCLUDE_DIR})
        
        # 여기에 프로젝트 소스 코드 및 빌드 설정을 추가합니다.
        add_executable(tls-client-sample openssl-tls-client-sample.cpp)
        # target_compile_options(tls-client-sample PRIVATE -Wall -Wextra -lssl -lcrypto)
        add_executable(tls-server-sample openssl-tls-server-sample.cpp)
        
        # 다음과 같이 라이브러리를 링크할 수 있습니다.
        target_link_libraries(tls-client-sample PUBLIC ${OPENSSL_LIBRARIES})
        target_link_libraries(tls-server-sample PUBLIC ${OPENSSL_LIBRARIES})

        target_link_libraries(tls-client PUBLIC ${OPENSSL_LIBRARIES})
        target_link_libraries(tls-server PUBLIC ${OPENSSL_LIBRARIES})
    else()
        message(FATAL_ERROR "OpenSSL not found. Please install OpenSSL.")
    endif()
endif()
# cross compile한 openssl을 링크해준다. # 추가 및 수정 예정
if(USE_OPENSSL AND ${CROSS_COMPILE})
    target_include_directories(tls-client PRIVATE #[[OpenSSL을 빌드 한 후 make install 까지 마친 후 include 위치]])
    target_link_directories(tls-client PRIVATE #[[OpenSSL을 빌드 한 후 make install 까지 마친 후 lib 위치]])
    target_link_libraries(tls-client PRIVATE ssl crypto)
    # example
    target_include_directories(tls-server PRIVATE libopenssl/include)
    target_link_directories(tls-server PRIVATE libopenssl/lib)
    target_link_libraries(tls-server PRIVATE ssl crypto)
endif()