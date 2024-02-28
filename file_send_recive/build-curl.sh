#! /bin/bash

# Set cross compiler and toolchain
export OPENSSL_ROOT_DIR=$PWD/openssl-3.2.1

export CROSS_COMPILE=arm-linux-gnueabihf
export CC=${CROSS_COMPILE}-gcc
export CXX=${CROSS_COMPILE}-g++
export AR=${CROSS_COMPILE}-ar
export AS=${CROSS_COMPILE}-as
export LD=${CROSS_COMPILE}-ld
export RANLIB=${CROSS_COMPILE}-ranlib
export NM=${CROSS_COMPILE}-nm

# Download curl Release version 8.6.0
wget https://github.com/curl/curl/releases/download/curl-8_6_0/curl-8.6.0.tar.gz
tar -zxvf curl-8.6.0.tar.gz
rm curl-8.6.0.tar.gz
cd curl-8.6.0/

# Paths of libcrypto.pc libssl.pc openssl.pc files for using pkg-config command
env PKG_CONFIG_PATH=$PKG_CONFIG_PATH:$OPENSSL_ROOT_DIR/tools/libopenssl/lib/pkgconfig

# The configure option should be modified based on your target environment
./configure --target=$CROSS_COMPILE \
            --host=$CROSS_COMPILE \
            --build=i586-pc-linux-gnu \
            --with-openssl=../libopenssl \
            --prefix=$PWD/tools/libcurl \
            --with-zlib=../zlib \
            --without-libpsl
            #만약 pkg-config를 설치 못 한다면
            #CPPFLAGS="-I/path/to/ssl/include" LDFLAGS="-L/path/to/ssl/lib"
            #LDFLAGS=-Wl,-R/usr/local/ssl/lib
            #https://curl.se/docs/install.html 에서 Unix의 마지막 부분에 openssl 관련 부분이 나와있음

# Build
make -j$(nproc)
make install

cd ..
cp -r curl-8.6.0/tools/libcurl $PWD/libcurl