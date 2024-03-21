#!/bin/bash

# export CC=arm-linux-gnueabihf-gcc
# export CXX=arm-linux-gnueabihf-g++
# export AR=arm-linux-gnueabihf-ar
# export RANLIB=arm-linux-gnueabihf-ranlib

wget https://github.com/madler/zlib/releases/download/v1.3.1/zlib-1.3.1.tar.gz
tar -zxvf zlib-1.3.1.tar.gz
rm zlib-1.3.1.tar.gz
cd zlib-1.3.1/

env CHOST=arm-linux-gnueabihf ./configure --prefix=${PWD}/zlib
make -j$(nproc)
make install

cd ..
cp -r zlib-1.3.1/zlib ${PWD}/zlib
rm -rf zlib-1.3.1