#! /bin/bash

# Set cross compiler and toolchain
export CROSS_COMPILE=arm-linux-gnueabihf
export CC=${CROSS_COMPILE}-gcc
export CXX=${CROSS_COMPILE}-g++
export AR=${CROSS_COMPILE}-ar
export AS=${CROSS_COMPILE}-as
export LD=${CROSS_COMPILE}-ld
export RANLIB=${CROSS_COMPILE}-ranlib
export NM=${CROSS_COMPILE}-nm

# Edit prefix path in .pc
# sed -i 's/\/zlib-1.3.1/\.\.\///' zlib/lib/pkgconfig/*.pc
# sed -i 's/\/zlib-1.3.1/\tools//' zlib/lib/pkgconfig/*.pc
# sed -i 's/\/openssl-3.2.1/\tools//' libopenssl/lib/pkgconfig/*.pc
sed -i 's/\/openssl-3.2.1//' libopenssl/lib/pkgconfig/*.pc # -> *


# https://curl.se/docs/install.html -> refer: Unix, Android, Cross compile
# ./configure --help -> check options
# Download curl Release version 8.6.0
wget https://github.com/curl/curl/releases/download/curl-8_6_0/curl-8.6.0.tar.gz
tar -zxvf curl-8.6.0.tar.gz
rm curl-8.6.0.tar.gz
cd curl-8.6.0/

# Paths of libcrypto.pc libssl.pc openssl.pc files for using pkg-config command
env PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$OPENSSL_ROOT_DIR/tools/libopenssl/lib/pkgconfig
# Paths of libcrypto.pc libssl.pc openssl.pc files for using pkg-config command (https://gist.github.com/lethee/a50d0cbb49cc95d5475dfdaea015556e)
# The configure option should be modified based on your target environment
# 애초에 build-openssl 할 때 prefix 위치 준 곳으로 .pc 파일에 prefix 경로가 설정됨
# pkg 경로는 그냥 -lcrypto, -lssl을 편하게 링킹하기 위한 수단인 것으로 생각된다.
# 저걸 안하면 --with-openssl=../libopenssl 을 해줘야함
# env PKG_CONFIG_PATH=$PWD/openssl-3.2.1/libopenssl/lib/pkgconfig ./configure --target=${CROSS_COMPILE} \ -> 이걸 선택한다면 위에 sed 명령어 주석처리하고 env 아래 경로도 주석처리!
env PKG_CONFIG_PATH=$PWD/../libopenssl/lib/pkgconfig ./configure --target=${CROSS_COMPILE} \
            --host=${CROSS_COMPILE} \
            --build=i586-pc-linux-gnu \
            --with-openssl \
            --prefix=${PWD}/libcurl \
            --with-zlib=${PWD}/../zlib \
            --without-libpsl
# --with-zlib=../zlib 는 절대 경로가 아니라며 에러 뜸
            #만약 pkg-config를 설치 못 한다면
            #CPPFLAGS="-I/path/to/ssl/include" LDFLAGS="-L/path/to/ssl/lib"
            #LDFLAGS=-Wl,-R/usr/local/ssl/lib
            #https://curl.se/docs/install.html 에서 Unix의 마지막 부분에 openssl 관련 부분이 나와있음

# Build
make -j$(nproc)
make install

cd ..
cp -r curl-8.6.0/libcurl ${PWD}/libcurl
rm -rf curl-8.6.0