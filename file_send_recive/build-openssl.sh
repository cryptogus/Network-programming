#!/bin/bash

# Set cross compiler
export CROSS_COMPILE=arm-linux-gnueabihf-

# Download openssl Release version 3.2.1
wget https://github.com/openssl/openssl/releases/download/openssl-3.2.1/openssl-3.2.1.tar.gz
tar -zxvf openssl-3.2.1.tar.gz
rm openssl-3.2.1.tar.gz
cd openssl-3.2.1/

# linux-generic32, Do not use the -m64 option because linux-generic32 compiles assuming a 32-bit situation. -> ./Configure LIST
# https://github.com/openssl/openssl/blob/master/INSTALL.md#manual-configuration
# https://github.com/openssl/openssl/blob/master/INSTALL.md#configuration-options
# The configure option should be modified based on your target environment
./Configure linux-armv4 --release --cross-compile-prefix=$CROSS_COMPILE --prefix=$PWD/tools/libopenssl

# Build
make -j$(nproc)
make install

cd ..
cp -r openssl-3.2.1/libopenssl ${PWD}/libopenssl
rm -rf openssl-3.2.1