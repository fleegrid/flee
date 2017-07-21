#!/bin/bash

set -e
set -u

BUILD=build/$(uname -s | tr "A-Z" "a-z")-$(uname -m | tr "A-Z" "a-z")

mkdir -p $BUILD
cd $BUILD
cmake ../..
make
