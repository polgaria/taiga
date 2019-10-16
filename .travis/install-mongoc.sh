#!/bin/sh
set -ex

wget https://github.com/mongodb/mongo-c-driver/releases/download/${MONGOC_VER}/mongo-c-driver-${MONGOC_VER}.tar.gz
tar xzf mongo-c-driver-${MONGOC_VER}.tar.gz

cd mongo-c-driver-${MONGOC_VER} && cd build
cmake -DCMAKE_BUILD_TYPE=${BUILD_TYPE} -DENABLE_AUTOMATIC_INIT_AND_CLEANUP=OFF ..

sudo make install -j$(nproc)
