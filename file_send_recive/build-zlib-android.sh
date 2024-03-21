#! /bin/bash

# Set cross compiler - Modifiable
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

# Download zlib Release version 1.3.1
wget https://github.com/madler/zlib/releases/download/v1.3.1/zlib-1.3.1.tar.gz
tar -zxvf zlib-1.3.1.tar.gz
rm zlib-1.3.1.tar.gz
cd zlib-1.3.1/

# The configure option - Modifiable
env CHOST=$TARGET_HOST ./configure --prefix=${PWD}/zlib

# Build & Install
make -j$(nproc)
make install

cd ..
cp -r zlib-1.3.1/zlib ${PWD}/zlib
rm -rf zlib-1.3.1