#!/bin/sh

set -e
set -u

BUILD_DIR=build/Debug/$(uname -s | tr "A-Z" "a-z")-$(uname -m | tr "A-Z" "a-z")

mkdir -p $BUILD_DIR
cd $BUILD_DIR
cmake -DCMAKE_BUILD_TYPE=Debug ../../.. && make
