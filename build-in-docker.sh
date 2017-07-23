#!/bin/sh

docker run --privileged --rm -v `pwd`:/src fleegrid/flee-build:170724 sh build.sh
