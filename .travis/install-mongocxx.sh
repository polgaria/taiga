#!/bin/sh
set -ex

git clone https://github.com/mongodb/mongo-cxx-driver.git \
    --branch releases/stable --depth 1
cd mongo-cxx-driver/build

cmake -DCMAKE_INSTALL_PREFIX=/usr/local -DCMAKE_BUILD_TYPE=${BUILD_TYPE} ..
sudo make EP_mnmlstc_core
make && sudo make install
