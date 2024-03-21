#!/bin/bash

# Set cross compiler and toolchain - Modifiable
export NDK=$PWD/android-ndk-r25c
export HOST_TAG=linux-x86_64
export MIN_SDK_VERSION=25
export CFLAGS="-Os"
export LDFLAGS="-Wl,-Bsymbolic"
export CORES=$((`nproc`+1))
export TOOLCHAIN=$NDK/toolchains/llvm/prebuilt/$HOST_TAG
export ANDROID_NDK_HOME=$NDK

PATH=$TOOLCHAIN/bin:$PATH

# arm(32bit)
export TARGET_HOST=armv7a-linux-androideabi
export ANDROID_ARCH=armeabi-v7a
export AR=$TOOLCHAIN/bin/llvm-ar
export CC=$TOOLCHAIN/bin/$TARGET_HOST$MIN_SDK_VERSION-clang
export AS=$CC
export CXX=$TOOLCHAIN/bin/$TARGET_HOST$MIN_SDK_VERSION-clang++
export LD=$TOOLCHAIN/bin/ld
export RANLIB=$TOOLCHAIN/bin/llvm-ranlib
export STRIP=$TOOLCHAIN/bin/llvm-strip

# Edit prefix path in .pc
sed -i 's/\/openssl-3.2.1//' libopenssl/lib/pkgconfig/*.pc

# Download curl Release version 8.6.0
wget https://github.com/curl/curl/releases/download/curl-8_6_0/curl-8.6.0.tar.gz
tar -zxvf curl-8.6.0.tar.gz
rm curl-8.6.0.tar.gz
cd curl-8.6.0/

# Paths of libcrypto.pc libssl.pc openssl.pc files for using pkg-config command - Modifiable
env PKG_CONFIG_PATH=$PWD/../libopenssl/lib/pkgconfig ./configure --target=${TARGET_HOST} \
            --host=${TARGET_HOST} \
            --build=i586-pc-linux-gnu \
            --with-openssl \
            --prefix=${PWD}/libcurl \
            --with-zlib=${PWD}/../zlib \
            --without-libpsl

# Build & Install
make -j$(nproc)
make install

cd ..
cp -r curl-8.6.0/libcurl ${PWD}/libcurl
rm -rf curl-8.6.0