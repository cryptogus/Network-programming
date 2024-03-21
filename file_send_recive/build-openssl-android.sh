#!/bin/bash

# Set cross compiler - Modifiable
# Android toolchain, https://github.com/openssl/openssl/blob/master/NOTES-ANDROID.md
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

# Download openssl Release version 3.2.1
wget https://github.com/openssl/openssl/releases/download/openssl-3.2.1/openssl-3.2.1.tar.gz
tar -zxvf openssl-3.2.1.tar.gz
rm openssl-3.2.1.tar.gz
cd openssl-3.2.1/

# The configure option should be modified based on your target environment - Modifiable

export ANDROID_NDK_ROOT=$NDK # https://github.com/openssl/openssl/blob/master/NOTES-ANDROID.md 참고, android-arm 옵션도 ./Configure LIST 참고
./Configure android-arm --prefix=${PWD}/libopenssl

# Build & Install
make -j$(nproc)
make install

cd ..
cp -r openssl-3.2.1/libopenssl ${PWD}/libopenssl
rm -rf openssl-3.2.1