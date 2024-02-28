#! /bin/bash

# Set cross compiler
CHOST=arm-linux-gnueabihf

wget https://github.com/madler/zlib/releases/download/v1.3.1/zlib-1.3.1.tar.gz
tar -zxvf zlib-1.3.1.tar.gz
rm zlib-1.3.1.tar.gz
cd zlib-1.3.1/

./configure --prefix=$PWD/tools/zlib
make -j$(nproc)
make install

cd ..
cp -r zlib-1.3.1/tools/zlib $PWD/zlib