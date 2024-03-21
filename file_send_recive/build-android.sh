#!/bin/bash

# 안드로이드 ndk 다운로드, https://developer.android.com/ndk/downloads?hl=ko
sudo apt install unzip
wget https://dl.google.com/android/repository/android-ndk-r25c-linux.zip # 현재 최신 LTS 버전 (r26c) 24.03.21
unzip android-ndk-r25c-linux.zip
rm android-ndk-r25c-linux.zip

# # -----------------------------------------------------------------------

# export NDK=$PWD/android-ndk-r25c
# export HOST_TAG=linux-x86_64
# export MIN_SDK_VERSION=23
# export CFLAGS="-Os"
# export LDFLAGS="-Wl,-Bsymbolic"
# export CORES=$((`nproc`+1))
# export TOOLCHAIN=$NDK/toolchains/llvm/prebuilt/$HOST_TAG
# export ANDROID_NDK_HOME=$NDK

# PATH=$TOOLCHAIN/bin:$PATH

# # arm(32bit)
# export TARGET_HOST=armv7a-linux-androideabi
# export ANDROID_ARCH=armeabi-v7a
# export AR=$TOOLCHAIN/bin/llvm-ar
# export CC=$TOOLCHAIN/bin/$TARGET_HOST$MIN_SDK_VERSION-clang
# export AS=$CC
# export CXX=$TOOLCHAIN/bin/$TARGET_HOST$MIN_SDK_VERSION-clang++
# export LD=$TOOLCHAIN/bin/ld
# export RANLIB=$TOOLCHAIN/bin/llvm-ranlib
# export STRIP=$TOOLCHAIN/bin/llvm-strip